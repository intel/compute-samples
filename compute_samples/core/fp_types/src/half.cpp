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
  const uint32_t float16_exp_shift = (23 - 10);
  const uint32_t float16_exp_mask = 0x7c00;
  const uint32_t float32_exp_mask = 0x7f800000;
  const uint32_t float16_mantissa_mask = 0x03ff;
  // -9 is needed because later we do multiplication of floats
  // 127 - 15 - 9 = 103, float exponent: 103 - 127 = -24
  // smallest half denorm = 2^-14 * 2^-10 = 2^-24
  // 2^-24 * float(mantissa = 0x0001) = 2^-24 * 1.0
  const uint32_t float16_to_32_bias_diff_denorm = ((127 - 15 - 9) << 23);
  const uint32_t float16_to_32_bias_diff = ((127 - 15) << 10);
  const uint32_t float16_sign_mask = 0x8000;
  const uint32_t exp = data & float16_exp_mask;
  const uint32_t mantissa = data & float16_mantissa_mask;
  const uint32_t sign = (data & float16_sign_mask) << 16;
  const uint32_t nan = (exp == float16_exp_mask ? float32_exp_mask : 0u);
  // 0.0
  if ((exp | mantissa) == 0) {
    return *reinterpret_cast<const float *>(&sign);
  }
  // normals
  if (exp != 0) {
    uint32_t tmp =
        (((exp + float16_to_32_bias_diff) | mantissa) << float16_exp_shift) |
        nan | sign;
    return *reinterpret_cast<float *>(&tmp);
  }
  // subnormals
  float tmpf =
      *reinterpret_cast<const float *>(&float16_to_32_bias_diff_denorm) *
      static_cast<float>(mantissa);
  *reinterpret_cast<uint32_t *>(&tmpf) |= sign;
  return tmpf;
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
