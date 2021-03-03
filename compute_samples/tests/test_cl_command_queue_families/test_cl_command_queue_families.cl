/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void copy_kernel(const global uint *input, global uint *output) {
  const size_t global_id = get_global_id(0);
  output[global_id] = input[global_id];
}
