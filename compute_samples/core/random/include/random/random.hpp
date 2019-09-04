/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_RANDOM_HPP
#define COMPUTE_SAMPLES_RANDOM_HPP

#include <vector>
#include <random>
#include <limits>
#include <algorithm>

namespace compute_samples {
template <typename T>
T generate_value(const T min, const T max, const int seed) {
  static std::default_random_engine engine(seed);
  std::uniform_int_distribution<T> distribution(min, max);
  return distribution(engine);
}

template <>
int8_t generate_value<int8_t>(const int8_t min, const int8_t max,
                              const int seed);
template <>
uint8_t generate_value<uint8_t>(const uint8_t min, const uint8_t max,
                                const int seed);
template <>
float generate_value<float>(const float min, const float max, const int seed);
template <>
double generate_value<double>(const double min, const double max,
                              const int seed);

template <typename T> T generate_value(const int seed) {
  const T min = std::numeric_limits<T>::min();
  const T max = std::numeric_limits<T>::max();
  return generate_value(min, max, seed);
}

template <typename T>
std::vector<T> generate_vector(const int size, const T min, const T max,
                               const int seed) {
  std::vector<T> data(size);
  std::generate(data.begin(), data.end(),
                [&] { return generate_value<T>(min, max, seed); });
  return data;
}

template <typename T>
std::vector<T> generate_vector(const int size, const int seed) {
  std::vector<T> data(size);
  std::generate(data.begin(), data.end(),
                [&] { return generate_value<T>(seed); });
  return data;
}

} // namespace compute_samples

#endif
