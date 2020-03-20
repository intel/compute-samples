/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void test_template_no_operands0(global int const *const input,
                                       global int *const output) {
  __asm__("barrier");
  output[get_global_id(0)] = 0;
}

kernel void test_template_no_operands1(global int const *const input,
                                       global int *const output) {
  const size_t tid = get_global_id(0);

  int tmp = input[tid];

  __asm__("barrier");

  tmp = tmp << 1;

  __asm__("barrier");

  tmp += 0xa5;

  __asm__("barrier");

  output[tid] = tmp;
}
