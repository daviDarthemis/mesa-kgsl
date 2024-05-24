/*
 * Copyright (C) 2023 Collabora, Ltd.
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "nir_builder.h"
#include "pan_context.h"

static bool
pass(nir_builder *b, nir_intrinsic_instr *intr, void *data)
{
   if (intr->intrinsic != nir_intrinsic_store_output)
      return false;

   unsigned *nr_cbufs = data;
   unsigned location = nir_intrinsic_io_semantics(intr).location;

   if (location >= FRAG_RESULT_DATA0 &&
       (location - FRAG_RESULT_DATA0) >= (*nr_cbufs)) {
      nir_instr_remove(&intr->instr);
      return true;
   } else {
      return false;
   }
}

bool
panfrost_nir_remove_fragcolor_stores(nir_shader *s, unsigned nr_cbufs)
{
   return nir_shader_intrinsics_pass(
      s, pass, nir_metadata_block_index | nir_metadata_dominance, &nr_cbufs);
}
