/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "random/random.hpp"

namespace compute_samples {
template <>
int8_t generate_value<int8_t>(const int8_t min, const int8_t max,
                              const int seed) {
  return static_cast<int8_t>(generate_value<int32_t>(min, max, seed));
}

template <>
uint8_t generate_value<uint8_t>(const uint8_t min, const uint8_t max,
                                const int seed) {
  return static_cast<uint8_t>(generate_value<uint32_t>(min, max, seed));
}

template <>
float generate_value<float>(const float min, const float max, const int seed) {
  static std::default_random_engine engine(seed);
  std::uniform_real_distribution<float> distribution(
      min, std::nextafter(max, std::numeric_limits<float>::max()));
  return distribution(engine);
}

template <>
double generate_value<double>(const double min, const double max,
                              const int seed) {
  static std::default_random_engine engine(seed);
  std::uniform_real_distribution<double> distribution(
      min, std::nextafter(max, std::numeric_limits<double>::max()));
  return distribution(engine);
}

} // namespace compute_samples
