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

#ifdef HAVE_SIMD8
__attribute__((intel_reqd_sub_group_size(8))) kernel void
test_constraints_immediate_simd8(global const int *const A,
                                 global int *const B) {
  const size_t wiID = get_global_id(0);

  __asm__("add (M1, 8) %0(0, 0)<1> %1(0, 0)<1;1,0> %2"
          : "=rw"(B[wiID])
          : "rw"(A[wiID]), "i"(CONST_ARGUMENT));
}
#endif // HAVE_SIMD8
