/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_constraints_multiple_outputs(global int *const d0) {
  const size_t tid = get_global_id(0);

  int dst1 = tid + 1, dst2 = tid + 2, dst3 = tid + 3;

  /* dst1 <<= 8; */
  /* dst2 <<= 16; */
  /* dst3 <<= 24; */
  __asm__("shl (M1, 16) %2(0,0)<1> %2(0,0)<1;1,0> 0x8:d\n"
          "shl (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> 0x10:d\n"
          "shl (M1, 16) %1(0,0)<1> %1(0,0)<1;1,0> 0x18:d\n"
          : "+rw"(dst2), "+rw"(dst3), "+rw"(dst1));

  d0[tid] = dst1 ^ dst2 ^ dst3;
}
