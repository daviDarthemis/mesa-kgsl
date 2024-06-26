/*
 * Copyright © 2020 Valve Corporation
 *
 * SPDX-License-Identifier: MIT
 */
#include "helpers.h"
#include <stdio.h>

using namespace aco;

BEGIN_TEST_TODO(todo)
   //!test!
   fprintf(output, "woops!\n");
END_TEST

BEGIN_TEST_FAIL(expect_fail)
   //!test!
   fprintf(output, "woops!\n");
END_TEST

BEGIN_TEST(simple._1)
   //! s_buffer_load_dwordx2 @s64(a)
   fprintf(output, "s_buffer_load_dwordx2 s[6:7]\n");
   //! s_add_u32 s#b0, s#a, 1
   //! s_addc_u32 s#b1, s#a1, 0
   //; success = int(b0) == 8
   fprintf(output, "s_add_u32 s8, s6, 1\n");
   fprintf(output, "s_addc_u32 s9, s7, 0\n");
   //! s_buffer_store_dwordx2 @s64(b)
   fprintf(output, "s_buffer_store_dwordx2 s[8:9]\n");
END_TEST

BEGIN_TEST(simple._2)
   //~gfx[67]! test gfx67
   //~gfx8! test gfx8
   //~gfx9! test gfx9
   //! test all
   for (int cls = GFX6; cls <= GFX7; cls++) {
      if (!set_variant((enum amd_gfx_level)cls))
         continue;
      fprintf(output, "test gfx67\n");
      fprintf(output, "test all\n");
   }

   if (set_variant("gfx8")) {
      fprintf(output, "test gfx8\n");
      fprintf(output, "test all\n");
   }

   if (set_variant("gfx9")) {
      fprintf(output, "test gfx9\n");
      fprintf(output, "test all\n");
   }
END_TEST

BEGIN_TEST(simple._3)
   //; funcs['test'] = lambda a: a
   //! @test(s_buffer_load_dwordx2) @s64(a)
   fprintf(output, "s_buffer_load_dwordx2 s[6:7]\n");
END_TEST
