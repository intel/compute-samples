/*
 * Copyright(c) 2019 Intel Corporation
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
