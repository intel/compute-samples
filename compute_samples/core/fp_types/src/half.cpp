/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "fp_types/half.hpp"

#include <cstring>

namespace compute_samples {

half::half(uint16_t v) : data(v) {}
half::half(float v) {
  const int float_exponent_offset = 127;
  const int half_exponent_offset = 15;
  const int max_half_exponent_value = 15;
  uint32_t tmp;
  std::memcpy(&tmp, &v, sizeof(uint32_t));
  const uint16_t sign = (tmp & 0x80000000) >> 31;
  uint16_t exponent = (tmp & 0x7f800000) >> 23;
  uint16_t mantissa = (tmp & 0x007fe000) >> 13;
  const uint16_t remainder = (tmp & 0x00001fff);

  if (exponent != 0 && exponent != 0x00ff) {
    if (exponent <= float_exponent_offset - half_exponent_offset) {
      // underflow to zero
      data = sign << 15;
      return;
    }
    if (exponent > float_exponent_offset + max_half_exponent_value) {
      // overflow to inf
      data = (sign << 15) | 0x7c00;
      return;
    }
    exponent -= float_exponent_offset - half_exponent_offset;
  } else if (exponent == 0x00ff) {
    exponent = 0x001f;
  }

  // round to nearest or even
  if (remainder > 0x1000 && mantissa != 0x03ff) {
    ++mantissa;
  } else if (remainder == 0x1000 && mantissa != 0x03ff && mantissa % 2 == 1) {
    ++mantissa;
  }
  data = (sign << 15) | (exponent << 10) | mantissa;
}
half::half(uint32_t v) : half(static_cast<float>(v)) {}
half::operator uint16_t() const { return data; }
half::operator float() const {
  const int float_exponent_offset = 127;
  const int half_exponent_offset = 15;
  const uint16_t sign = (data & 0x8000) >> 15;
  uint16_t exponent = (data & 0x7c00) >> 10;
  const uint16_t mantissa = (data & 0x03ff);
  exponent += float_exponent_offset - half_exponent_offset;
  uint32_t float_binary = (sign << 31) | (exponent << 23) | (mantissa << 13);
  float tmp_f;
  std::memcpy(&tmp_f, &float_binary, sizeof(uint32_t));
  return tmp_f;
}
half::operator uint32_t() const {
  return static_cast<uint32_t>(static_cast<float>(*this));
}
half half::operator-() const {
  return half(static_cast<uint16_t>(data ^ 0x8000));
}
bool half::operator==(const half &rhs) const { return data == rhs.data; }
bool half::nan_sensitive_eq(const half &rhs) const {
  const bool this_nan = (data & 0x7c00) == 0x7c00 && (data & 0x03ff) != 0;
  const bool rhs_nan =
      (rhs.data & 0x7c00) == 0x7c00 && (rhs.data & 0x03ff) != 0;
  if (this_nan && rhs_nan) {
    return true;
  }
  return data == rhs.data;
}
half &half::operator+=(const half &rhs) {
  const half tmp = half(static_cast<float>(*this) + static_cast<float>(rhs));
  data = static_cast<uint16_t>(tmp);
  return *this;
}
half &half::operator*=(const half &rhs) {
  const half tmp = half(static_cast<float>(*this) * static_cast<float>(rhs));
  data = static_cast<uint16_t>(tmp);
  return *this;
}
half operator+(half lhs, const half &rhs) {
  lhs += rhs;
  return lhs;
}
half operator*(half lhs, const half &rhs) {
  lhs *= rhs;
  return lhs;
}
float &operator+=(float &lhs, const half &rhs) {
  lhs += static_cast<float>(rhs);
  return lhs;
}

template <> int bits<half>() { return 16; }

template <> std::string to_cl_c_string<half>() { return "half"; };
template <> std::string to_cl_c_string<half2>() { return "half2"; };
template <> std::string to_cl_c_string<half4>() { return "half4"; };
template <> std::string to_cl_c_string<half8>() { return "half8"; };
} // namespace compute_samples
