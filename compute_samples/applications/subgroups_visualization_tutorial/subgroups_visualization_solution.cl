/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

// By default, this particular kernel happens to be compiled for
// a subgroup size of 32.
//
// Because the application code is setup to use a work group
// size of 128 by default, with a subgroup size of 32 you will
// see four subgroup IDs per work group.
//
// If the application code is setup to use a different work
// group size, particularly a larger work group size, you may
// want to change the scale factor to avoid overflowing the
// 8-bit color data.

__kernel void Visualize(write_only image2d_t image) {
  uint scale = 32;
  uint color = get_sub_group_id() * scale + get_group_id(1) % 8;

  int2 coord = (int2)(get_global_id(0), get_global_id(1));
  write_imageui(image, coord, color);
}
