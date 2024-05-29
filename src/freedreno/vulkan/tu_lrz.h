/*
 * Copyright © 2016 Red Hat.
 * Copyright © 2016 Bas Nieuwenhuizen
 * SPDX-License-Identifier: MIT
 *
 * based in part on anv driver which is:
 * Copyright © 2015 Intel Corporation
 */

#ifndef TU_LRZ_H
#define TU_LRZ_H

#include "tu_common.h"

enum tu_lrz_force_disable_mask {
   TU_LRZ_FORCE_DISABLE_LRZ = 1 << 0,
   TU_LRZ_FORCE_DISABLE_WRITE = 1 << 1,
   TU_LRZ_READS_DEST = 1 << 2,              /* Blend/logicop/colormask, etc */
};

enum tu_lrz_direction {
   TU_LRZ_UNKNOWN,
   /* Depth func less/less-than: */
   TU_LRZ_LESS,
   /* Depth func greater/greater-than: */
   TU_LRZ_GREATER,
};

struct tu_lrz_state
{
   /* Depth/Stencil image currently on use to do LRZ */
   const struct tu_image_view *image_view;
   VkClearValue depth_clear_value;
   /* If LRZ is in invalid state we cannot use it until depth is cleared */
   bool valid : 1;
   /* Allows to temporary disable LRZ */
   bool enabled : 1;
   bool fast_clear : 1;
   bool gpu_dir_tracking : 1;
   /* Continue using old LRZ state (LOAD_OP_LOAD of depth) */
   bool reuse_previous_state : 1;
   enum tu_lrz_direction prev_direction;
};

/* Layout of LRZ fast-clear buffer templated on the generation, the
 * members are as follows:
 * - fc1: The first FC buffer, always present. This may contain multiple
 *        sub-buffers with _a/_b suffixes for concurrent binning which
 *        can be checked using HAS_CB.
 * - fc2: The second FC buffer, used for bidirectional LRZ and only present
 *        when HAS_BIDIR set. It has suffixes for CB like fc1.
 * - metadata: Metadata buffer for LRZ fast-clear. The contents are not
 *             always known, since they're handled by the hardware.
 */
template <chip CHIP>
struct tu_lrzfc_layout;

template <>
struct PACKED tu_lrzfc_layout<A6XX> {
   static const bool HAS_BIDIR = false;
   static const bool HAS_CB = false;
   static const size_t FC_SIZE = 512;

   uint8_t fc1[FC_SIZE];
   union {
      struct {
         uint8_t dir_track;
         uint8_t _pad_;
         uint32_t gras_lrz_depth_view;
      };
      uint8_t metadata[6];
   };
};

template <>
struct PACKED tu_lrzfc_layout<A7XX> {
   static const bool HAS_BIDIR = true;
   static const bool HAS_CB = true;
   static const size_t FC_SIZE = 1024;

   union {
      struct {
         uint8_t fc1_a[FC_SIZE];
         uint8_t fc1_b[FC_SIZE];
      };
      uint8_t fc1[FC_SIZE * 2];
   };
   uint8_t metadata[512];
   union {
      struct {
         uint8_t fc2_a[FC_SIZE];
         uint8_t fc2_b[FC_SIZE];
      };
      uint8_t fc2[FC_SIZE * 2];
   };
};

template <chip CHIP>

void
tu6_emit_lrz(struct tu_cmd_buffer *cmd, struct tu_cs *cs);

template <chip CHIP>
void
tu_disable_lrz(struct tu_cmd_buffer *cmd, struct tu_cs *cs,
               struct tu_image *image);

template <chip CHIP>
void
tu_lrz_clear_depth_image(struct tu_cmd_buffer *cmd,
                         struct tu_image *image,
                         const VkClearDepthStencilValue *pDepthStencil,
                         uint32_t rangeCount,
                         const VkImageSubresourceRange *pRanges);

template <chip CHIP>
void
tu_lrz_begin_renderpass(struct tu_cmd_buffer *cmd);

template <chip CHIP>
void
tu_lrz_begin_resumed_renderpass(struct tu_cmd_buffer *cmd);

void
tu_lrz_begin_secondary_cmdbuf(struct tu_cmd_buffer *cmd);

template <chip CHIP>
void
tu_lrz_tiling_begin(struct tu_cmd_buffer *cmd, struct tu_cs *cs);

template <chip CHIP>
void
tu_lrz_tiling_end(struct tu_cmd_buffer *cmd, struct tu_cs *cs);

template <chip CHIP>
void
tu_lrz_sysmem_begin(struct tu_cmd_buffer *cmd, struct tu_cs *cs);


template <chip CHIP>
void
tu_lrz_sysmem_end(struct tu_cmd_buffer *cmd, struct tu_cs *cs);


template <chip CHIP>
void
tu_lrz_disable_during_renderpass(struct tu_cmd_buffer *cmd);

#endif /* TU_LRZ_H */
