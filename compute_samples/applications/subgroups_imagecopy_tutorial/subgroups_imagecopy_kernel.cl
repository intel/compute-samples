/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__kernel void ImageCopy(write_only image2d_t dstImage,
                        read_only image2d_t srcImage) {
  // This kernel is currently set up to copy the image using
  // standard OpenCL read_image() and write_image() built-in
  // functions.  For this exercise, change the kernel to copy
  // the image using subgroup image block reads and writes
  // instead.

  // It is possible to change just the read or just the write
  // to use subgroup image block reads or writes, but it is
  // likely easier to change both at the same time.

  // Remember that the x-coordinate for the subgroup block
  // reads and writes is a byte coordinate, not an image
  // element coordinate.  Because the images used in this
  // exercise are 8-bit single-channel images the two can be
  // used interchangeably, but this is not always true!

  int work_group_pixel_offset =
      get_group_id(0) * get_enqueued_local_size(0) * 4;
  int work_item_pixel_offset = work_group_pixel_offset + get_local_id(0) * 4;

  for (uint pixel = 0; pixel < 4; pixel++) {
    int2 coord = (int2)(work_item_pixel_offset + pixel, get_global_id(1));

    uint4 color = read_imageui(srcImage, coord);

    write_imageui(dstImage, coord, color);
  }
}
