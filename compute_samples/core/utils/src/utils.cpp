/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
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
