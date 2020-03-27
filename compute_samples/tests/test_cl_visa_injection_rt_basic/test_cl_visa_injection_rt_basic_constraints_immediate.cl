/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_constraints_immediate(global int *const d0) {
  const size_t tid = get_global_id(0);

  int dst = d0[tid];

  /* dst |= 1 << (sizeof(int) * 8 - 1) | 0x42; */
  __asm__("or (M1, 16) %0(0,0)<1> %1 %0(0,0)<1;1,0>"
          : "+rw"(dst)
          : "i"(1 << (sizeof(int) * 8 - 1) | 0x42));

  d0[tid] = dst;
}
