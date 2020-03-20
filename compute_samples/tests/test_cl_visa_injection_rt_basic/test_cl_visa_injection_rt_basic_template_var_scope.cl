/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_template_var_scope(global int *const d0, global int const *const s1,
                        global int const *const s2) {
  const size_t tid = get_global_id(0);

  int dst = s2[1];
  const int src1 = s1[tid];
  const int src2 = s2[tid];

  // dst = src2 * dst - src1;
  // dst = dst + src1 * src2;
  __asm__ volatile("{\n"
                   ".decl tmp v_type=G type=d num_elts=16 align=GRF\n"
                   "mul (M1, 16) tmp(0,0)<1> %1(0,0)<1;1,0> %0(0,0)<1;1,0>\n"
                   "add (M1, 16) %0(0,0)<1> tmp(0,0)<1;1,0> (-)%2(0,0)<1;1,0>\n"
                   "}\n"
                   : "+rw"(dst)
                   : "rw"(src2), "rw"(src1));
  __asm__ volatile("{\n"
                   ".decl tmp v_type=G type=d num_elts=16 align=GRF\n"
                   "mul (M1, 16) tmp(0,0)<1> %2(0,0)<1;1,0> %1(0,0)<1;1,0>\n"
                   "add (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> tmp(0,0)<1;1,0>\n"
                   "}\n"
                   : "+rw"(dst)
                   : "rw"(src2), "rw"(src1));

  d0[tid] = dst;
}
