/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_template_no_outputs(global int *const d0) {
  const size_t tid = get_global_id(0);

  int tmp = tid << 4;
  // tmp += 0x42;
  __asm__ volatile("add (M1, 16) %1(0,0)<1> %1(0,0)<1;1,0> %0"
                   :
                   : "rw"(0x42), "rw"(tmp));
  d0[tid] = tmp;
}
