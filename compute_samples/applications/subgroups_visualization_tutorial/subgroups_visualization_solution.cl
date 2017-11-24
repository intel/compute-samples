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
