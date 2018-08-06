/*
 * Copyright(c) 2018 Intel Corporation
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

#ifndef COMPUTE_SAMPLES_BMP_HPP
#define COMPUTE_SAMPLES_BMP_HPP

#include <cstdint>
#include <stddef.h>

namespace compute_samples {

class BmpUtils {
public:
  static bool save_image_as_bmp(uint32_t *ptr, int width, int height,
                                const char *file_name);
  static bool save_image_as_bmp_32fc4(float *ptr, float scale, int width,
                                      int height, const char *file_name);
  static bool save_image_as_bmp_8u(uint8_t *ptr, int width, int height,
                                   const char *file_name);

  static bool load_bmp_image(uint8_t *&data, int &width, int &height,
                             int &pitch, uint16_t &bits_per_pixel,
                             const char *file_name);
  static bool load_bmp_image_8u(uint8_t *&data, int &width, int &height,
                                const char *file_name);
};
} // namespace compute_samples

#endif
