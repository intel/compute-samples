/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void usm_hello_world_kernel(const global uint *input,
                                   global uint *output) {
  const int tid = get_global_id(0);
  output[tid] = input[tid];
}
