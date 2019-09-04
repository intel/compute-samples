/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__kernel void Visualize(write_only image2d_t image) {
  // This kernel is currently set up to display the work group
  // ID.  For this exercise, change the kernel so it displays
  // the subgroup ID instead, then experiment with different
  // local work sizes or the required subgroup size attribute
  // to see how this changes the assignment of global IDs to
  // subgroups.

  // Hints:
  // * The call to get the work group ID is get_group_id().
  // * The call to get the subgroup ID is get_sub_group_id().
  // * Replace the call to get_group_id(0) with get_sub_group_id().
  // * Note that there are no parameters to get_sub_group_id(),
  //   because subgroups are unconditionally 1-dimensional.

  // The color of the image in the picture encodes the group
  // ID in the x and y dimensions.  Because the x dimension
  // is scaled by a scale factor, the larger changes in color
  // show the x dimension, and the smaller changes in color
  // show the y dimension.

  uint scale = 32;
  uint color = get_group_id(0) * scale + get_group_id(1) % 8;

  int2 coord = (int2)(get_global_id(0), get_global_id(1));
  write_imageui(image, coord, color);
}
