/*
 * Copyright © 2015 Broadcom
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "vc4_qir.h"
#include "kernel/vc4_packet.h"
#include "compiler/nir/nir_builder.h"

/** @file vc4_nir_lower_txf_ms.c
 * Walks the NIR generated by TGSI-to-NIR to lower its nir_texop_txf_ms
 * coordinates to do the math necessary and use a plain nir_texop_txf instead.
 *
 * MSAA textures are laid out as 32x32-aligned blocks of RGBA8888 or Z24S8.
 * We can't load them through the normal sampler path because of the lack of
 * linear support in the hardware.  So, we treat MSAA textures as a giant UBO
 * and do the math in the shader.
 */

static nir_def *
vc4_nir_lower_txf_ms_instr(nir_builder *b, nir_instr *instr, void *data)
{
        nir_tex_instr *txf_ms = nir_instr_as_tex(instr);
        const struct vc4_compile *c = data;

        nir_tex_instr *txf = nir_tex_instr_create(c->s, 1);
        txf->op = nir_texop_txf;
        txf->texture_index = txf_ms->texture_index;
        txf->coord_components = txf_ms->coord_components;
        txf->is_shadow = txf_ms->is_shadow;
        txf->is_new_style_shadow = txf_ms->is_new_style_shadow;
        txf->dest_type = txf_ms->dest_type;

        nir_def *coord = NULL, *sample_index = NULL;
        for (int i = 0; i < txf_ms->num_srcs; i++) {
                switch (txf_ms->src[i].src_type) {
                case nir_tex_src_coord:
                        coord = txf_ms->src[i].src.ssa;
                        break;
                case nir_tex_src_ms_index:
                        sample_index = txf_ms->src[i].src.ssa;
                        break;
                default:
                        unreachable("Unknown txf_ms src\n");
                }
        }
        assert(coord);
        assert(sample_index);

        nir_def *x = nir_channel(b, coord, 0);
        nir_def *y = nir_channel(b, coord, 1);

        uint32_t tile_w = 32;
        uint32_t tile_h = 32;
        uint32_t tile_w_shift = 5;
        uint32_t tile_h_shift = 5;
        uint32_t tile_size = (tile_h * tile_w *
                              VC4_MAX_SAMPLES * sizeof(uint32_t));
        unsigned unit = txf_ms->texture_index;
        uint32_t w = align(c->key->tex[unit].msaa_width, tile_w);
        uint32_t w_tiles = w / tile_w;

        nir_def *x_tile = nir_ushr_imm(b, x, tile_w_shift);
        nir_def *y_tile = nir_ushr_imm(b, y, tile_h_shift);
        nir_def *tile_addr = nir_iadd(b,
                                          nir_imul_imm(b, x_tile, tile_size),
                                          nir_imul_imm(b, y_tile, w_tiles *
                                                                  tile_size));
        nir_def *x_subspan = nir_iand_imm(b, x, (tile_w - 1) & ~1);
        nir_def *y_subspan = nir_iand_imm(b, y, (tile_h - 1) & ~1);
        nir_def *subspan_addr = nir_iadd(b,
                                             nir_imul_imm(b, x_subspan,
                                                          2 * VC4_MAX_SAMPLES * sizeof(uint32_t)),
                                             nir_imul_imm(b, y_subspan,
                                                          tile_w * VC4_MAX_SAMPLES *
                                                          sizeof(uint32_t)));

        nir_def *pixel_addr = nir_ior(b,
                                          nir_iand_imm(b,
                                                       nir_ishl_imm(b, x, 2),
                                                       1 << 2),
                                          nir_iand_imm(b,
                                                       nir_ishl_imm(b, y, 3),
                                                       1 << 3));

        nir_def *sample_addr = nir_ishl_imm(b, sample_index, 4);

        nir_def *addr = nir_iadd(b,
                                     nir_ior(b, sample_addr, pixel_addr),
                                     nir_iadd(b, subspan_addr, tile_addr));

        txf->src[0] = nir_tex_src_for_ssa(nir_tex_src_coord,
                                          nir_vec2(b, addr, nir_imm_int(b, 0)));
        nir_def_init(&txf->instr, &txf->def, 4, 32);
        nir_builder_instr_insert(b, &txf->instr);

        return &txf->def;
}

static bool
vc4_nir_lower_txf_ms_filter(const nir_instr *instr, const void *data)
{
        return (instr->type == nir_instr_type_tex &&
                nir_instr_as_tex(instr)->op == nir_texop_txf_ms);
}

void
vc4_nir_lower_txf_ms(nir_shader *s, struct vc4_compile *c)
{
        nir_shader_lower_instructions(s,
                                      vc4_nir_lower_txf_ms_filter,
                                      vc4_nir_lower_txf_ms_instr,
                                      c);
}
