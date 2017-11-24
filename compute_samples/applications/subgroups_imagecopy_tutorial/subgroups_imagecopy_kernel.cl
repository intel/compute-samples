/*
 * Copyright(c) 2017 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
