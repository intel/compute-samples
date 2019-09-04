/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void template_kernel(const global char *input, global char *output) {
  const int tid = get_global_id(0);
  output[tid] = input[tid];
}
