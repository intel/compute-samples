/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_UTILS_HPP
#define COMPUTE_SAMPLES_UTILS_HPP

#include <algorithm>
#include <vector>
#include <string>
#include <tuple>
#include <cassert>
#include <cstring>

namespace compute_samples {
std::string load_text_file(const std::string &file_path);
void save_text_file(const std::string data, const std::string &file_path);

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

template <typename OUTPUT_TYPE, typename INPUT_TYPE>
std::vector<OUTPUT_TYPE> unpack_vector(const std::vector<INPUT_TYPE> &input,
                                       const int step) {
  static_assert(sizeof(INPUT_TYPE) >= sizeof(OUTPUT_TYPE),
                "Size of INPUT_TYPE must be greater or equal to OUTPUT_TYPE");
  assert(step > 0);

  const size_t size_ratio = sizeof(INPUT_TYPE) / sizeof(OUTPUT_TYPE);

  std::vector<OUTPUT_TYPE> output(input.size() * size_ratio);

  const size_t columns = static_cast<size_t>(step);
  const size_t rows = input.size() / columns;

  for (size_t y = 0; y < rows; ++y) {
    for (size_t x = 0; x < columns; ++x) {
      std::vector<OUTPUT_TYPE> tmp(size_ratio);
      const int input_offset = y * columns + x;
      std::memcpy(tmp.data(), &input[input_offset], sizeof(INPUT_TYPE));
      size_t output_offset = x + y * columns * size_ratio;
      for (size_t i = 0; i < tmp.size(); ++i) {
        output[output_offset] = tmp[i];
        output_offset += columns;
      }
    }
  }
  return output;
}

template <typename In, typename Out>
std::vector<std::tuple<In, Out>> zip(const std::vector<In> in,
                                     const std::vector<Out> out) {
  std::vector<std::tuple<In, Out>> temp(in.size());
  std::transform(in.begin(), in.end(), out.begin(), temp.begin(),
                 [](const In &a, const Out &b) { return std::tie(a, b); });
  return temp;
}

std::string uuid_to_string(const uint8_t uuid[]);

void erase_substring(std::string &string, const std::string &substring);
void erase_substrings(std::string &string,
                      const std::vector<std::string> &substrings);
std::vector<std::string> split_string(const std::string &string,
                                      const std::string &delimeter);
std::string join_strings(const std::vector<std::string> &tokens,
                         const std::string &delimeter);

} // namespace compute_samples

#endif
