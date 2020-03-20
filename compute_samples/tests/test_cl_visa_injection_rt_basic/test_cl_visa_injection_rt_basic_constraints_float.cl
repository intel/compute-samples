/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_constraints_float(global int *const d, global const int *const s) {
  const size_t tid = get_global_id(0);

  float sf = (float)s[tid];

  // sf = -sf;
  __asm__("mov (M1, 16) %0(0,0)<1> (-)%0(0,0)<1;1,0>" : "+rw"(sf));

  d[tid] = (int)sf;
}
