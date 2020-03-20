/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_constraints_load_store_pointer(global int *const d,
                                    global const int *const s) {
  const size_t tid = get_global_id(0);

  int src, dst;

  /* src = s[tid]; */
  __asm__ volatile("svm_gather.4.1 (M1, 16) %1.0 %0.0"
                   : "=rw"(src)
                   : "rw"(&s[tid]));
  dst = src + tid;

  /* d[tid] = dst; */
  __asm__ volatile("svm_scatter.4.1 (M1, 16) %0.0 %1.0"
                   :
                   : "rw"(&d[tid]), "rw"(dst));
}
