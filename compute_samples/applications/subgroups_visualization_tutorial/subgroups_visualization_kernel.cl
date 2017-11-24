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
