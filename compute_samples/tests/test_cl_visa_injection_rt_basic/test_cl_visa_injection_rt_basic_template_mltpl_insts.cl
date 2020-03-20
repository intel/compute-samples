/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_template_mltpl_insts(global int *const d0, global int const *const s1,
                          global int const *const s2) {
  const size_t tid = get_global_id(0);

  int dst = s1[0];
  const int src1 = s1[tid];
  const int src2 = s2[tid];
  const int tmp1 = src2 * src1;
  const int tmp2 = src2 - src1;

  // dst = dst - tmp1;
  // dst = dst * tmp2;
  __asm__ volatile("add (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> (-)%2(0,0)<1;1,0>\n"
                   "mul (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> %1(0,0)<1;1,0>"
                   : "+rw"(dst)
                   : "rw"(tmp2), "rw"(tmp1));

  d0[tid] = dst;
}
