/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

kernel void commands_aggregation(global uint *buffer,
                                 const uint kernel_offset) {
  const uint id = get_global_id(0);
  const uint size = get_global_size(0);
  const ulong number_of_iterations = 1000000;
  for (uint i = 0; i < number_of_iterations; ++i) {
    buffer[kernel_offset * size + id]++;
  }
}
