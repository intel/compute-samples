/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_template_no_inputs(global int *const d0) {
  const size_t tid = get_global_id(0);

  d0[tid] = 64 - tid;

  int dst = d0[tid];
  // dst = dst * dst;
  __asm__("mul (M1,16) %0(0,0)<1> %0(0,0)<1;1,0> %0(0,0)<1;1,0>" : "+rw"(dst));
  d0[tid] = dst;
}
