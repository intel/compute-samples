/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_kernel(const global uint *M, const global uint *V, global uint *dst) {
  const int x = get_global_id(0);
  const int wind = get_global_size(0);

  dst[x] = 0;
  for (int i = 0; i < wind; ++i) {
    dst[x] += M[x * wind + i] * V[i];
  }
}

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_kernel_unroll(const global uint *M, const global uint *V,
                   global uint *dst) {
  const int x = get_global_id(0);
  const int wind = get_global_size(0);

  dst[x] = 0;
#pragma unroll
  for (int i = 0; i < wind; ++i) {
    dst[x] += M[x * wind + i] * V[i];
  }
}

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_kernel_unroll_with_asm(const global uint *M, const global uint *V,
                            global uint *dst) {
  const int x = get_global_id(0);
  const int wind = get_global_size(0);

  dst[x] = 0;
#pragma unroll
  for (int i = 0; i < wind; ++i) {
    // dst[x] += M[x * wind + i] * V[i];
    uint mx_el = M[x * wind + i];
    uint vec_el = V[i];
    uint res = 0;
    __asm__ volatile("mul (M1, 16) %0(0,0)<1> %1(0,0)<1;1,0> %2(0,0)<0;1,0>"
                     : "=rw"(res)
                     : "rw"(mx_el), "rw"(vec_el));
    dst[x] += res;
  }
}
