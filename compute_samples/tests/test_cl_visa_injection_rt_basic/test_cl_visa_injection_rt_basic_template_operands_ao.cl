/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_template_operands_arbitrary_order(global int *const d0,
                                       global int const *const s1) {
  const size_t tid = get_global_id(0);

  int dst = 0x42;
  const int src1 = s1[tid];

  // dst = src1 - dst;
  __asm__("add (M1,16) %0(0,0)<1> %1(0,0)<1;1,0> (-)%0(0,0)<1;1,0>"
          : "+rw"(dst)
          : "rw"(src1));

  d0[tid] = dst;
}
