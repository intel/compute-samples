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

#include "utils/utils.hpp"
#include "logging/logging.hpp"

#include <iostream>
#include <fstream>

namespace compute_samples {
std::vector<uint8_t> load_binary_file(const std::string &file_path) {
  LOG_ENTER_FUNCTION
  LOG_DEBUG << "File path: " << file_path;
  std::ifstream stream(file_path, std::ios::in | std::ios::binary);

  std::vector<uint8_t> binary_file;
  if (!stream.good()) {
    LOG_ERROR << "Failed to load binary file: " << file_path;
    LOG_EXIT_FUNCTION
    return binary_file;
  }

  size_t length = 0;
  stream.seekg(0, stream.end);
  length = static_cast<size_t>(stream.tellg());
  stream.seekg(0, stream.beg);
  LOG_DEBUG << "Binary file length: " << length;

  binary_file.resize(length);
  stream.read(reinterpret_cast<char *>(binary_file.data()), length);
  LOG_DEBUG << "Binary file loaded";

  LOG_EXIT_FUNCTION
  return binary_file;
}

void save_binary_file(const std::vector<uint8_t> &data,
                      const std::string &file_path) {
  LOG_ENTER_FUNCTION
  LOG_DEBUG << "File path: " << file_path;

  std::ofstream stream(file_path, std::ios::out | std::ios::binary);
  stream.write(reinterpret_cast<const char *>(data.data()),
               size_in_bytes(data));

  LOG_EXIT_FUNCTION
}

} // namespace compute_samples
