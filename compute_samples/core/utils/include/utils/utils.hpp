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

#ifndef COMPUTE_SAMPLES_UTILS_HPP
#define COMPUTE_SAMPLES_UTILS_HPP

#include <vector>
#include <string>
#include <cassert>
#include <cstring>

namespace compute_samples {
std::vector<uint8_t> load_binary_file(const std::string &file_path);
void save_binary_file(const std::vector<uint8_t> &data,
                      const std::string &file_path);

template <typename T> int size_in_bytes(const std::vector<T> &v) {
  return static_cast<int>(sizeof(T) * v.size());
}

template <typename OUTPUT_TYPE, typename INPUT_TYPE>
std::vector<OUTPUT_TYPE> pack_vector(const std::vector<INPUT_TYPE> &input,
                                     const int step) {
  static_assert(sizeof(OUTPUT_TYPE) >= sizeof(INPUT_TYPE),
                "Size of OUTPUT_TYPE must be greater or equal to INPUT_TYPE");
  assert(step > 0);

  const size_t size_ratio = sizeof(OUTPUT_TYPE) / sizeof(INPUT_TYPE);

  std::vector<OUTPUT_TYPE> output(input.size() / size_ratio);

  const size_t columns = static_cast<size_t>(step);
  const size_t rows = output.size() / columns;

  for (size_t y = 0; y < rows; ++y) {
    for (size_t x = 0; x < columns; ++x) {
      std::vector<INPUT_TYPE> tmp(size_ratio);
      for (size_t i = 0; i < tmp.size(); ++i) {
        const size_t row_offset = y * columns * size_ratio;
        const size_t column_offset = x;
        const size_t element_offset = i * columns;
        const size_t input_offset = row_offset + column_offset + element_offset;
        tmp[i] = input[input_offset];
      }
      const size_t output_offset = y * columns + x;
      std::memcpy(&output[output_offset], tmp.data(), sizeof(OUTPUT_TYPE));
    }
  }

  return output;
}

} // namespace compute_samples

#endif
