/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_template_empty0(global int const *const input,
                                 global int *const output) {
  __asm__("");
  output[get_global_id(0)] = 0;
}

kernel void test_template_empty1(global int const *const input,
                                 global int *const output) {
  const size_t tid = get_global_id(0);

  int tmp = input[tid];

  __asm__("");

  tmp = tmp << 2;

  __asm__("");

  tmp += 1;

  __asm__("");

  output[tid] = tmp;
}
