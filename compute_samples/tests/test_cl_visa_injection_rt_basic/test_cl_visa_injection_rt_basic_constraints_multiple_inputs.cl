/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_constraints_multiple_inputs(global int *const d0,
                                 global const int *const s0,
                                 global const int *const s1) {
  const size_t tid = get_global_id(0);

  int dst = d0[tid];
  const int src0 = s0[tid];
  const int src1 = s1[tid];

  /* dst = src1 - 0xa55a; */
  __asm__("add (M1, 16) %1(0,0)<1> %2(0,0)<1;1,0> %0"
          :
          : "rw"(0xffff5aa6), "=rw"(dst), "rw"(src1), "rw"(src0));

  d0[tid] = dst;
}
