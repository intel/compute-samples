/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void copy_kernel(global uchar *destination, const global uchar *source,
                        const uchar element_size) {
  const int tid = get_global_id(0);
  for (int i = 0; i < element_size; ++i) {
    destination[tid * element_size + i] = source[tid * element_size + i];
  }
}

kernel void indirect_copy_kernel(global uintptr_t *destination,
                                 const global uintptr_t *source) {
  const int tid = get_global_id(0);
  const uchar *s = (uchar *)(source[tid]);
  uchar *d = (uchar *)(destination[tid]);
  *d = *s;
}

kernel void atomic_copy_kernel(global atomic_int *destination,
                               global atomic_int *source) {
  const int tid = get_global_id(0);
  atomic_store(&destination[tid], atomic_load(&source[tid]));
}
