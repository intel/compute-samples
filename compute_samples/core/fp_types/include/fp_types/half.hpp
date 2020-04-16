/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_FP_TYPES_HALF_HPP
#define COMPUTE_SAMPLES_FP_TYPES_HALF_HPP

#include <cstdint>
#include <limits>
#include <type_traits>
#include "ocl_utils/ocl_utils.hpp"

namespace compute_samples {
class half {
public:
  half() = default;
  explicit half(uint16_t v);
  explicit half(float v);
  explicit half(uint32_t v);
  explicit operator uint16_t() const;
  explicit operator float() const;
  explicit operator uint32_t() const;
  half operator-() const;
  bool operator==(const half &rhs) const;
  bool nan_sensitive_eq(const half &rhs) const;
  half &operator+=(const half &rhs);
  half &operator*=(const half &rhs);

private:
  uint16_t data;
};
half operator+(half lhs, const half &rhs);
half operator*(half lhs, const half &rhs);

float &operator+=(float &lhs, const half &rhs);

template <typename T> int bits();

using half2 = union alignas(4) hf2 { half s[2]; };
using half4 = union alignas(8) hf4 { half s[4]; };
using half8 = union alignas(16) hf8 { half s[8]; };

static_assert(sizeof(half) == 2, "bad size of half type");
static_assert(sizeof(half2) == 4, "bad size of half2 type");
static_assert(sizeof(half4) == 8, "bad size of half4 type");
static_assert(sizeof(half8) == 16, "bad size of half8 type");

template <> struct cl_scalar_type<half> { using type = half; };
template <> struct cl_scalar_type<half2> { using type = half; };
template <> struct cl_scalar_type<half4> { using type = half; };
template <> struct cl_scalar_type<half8> { using type = half; };

} // namespace compute_samples

namespace std {
template <> class numeric_limits<compute_samples::half> {
public:
  static compute_samples::half min() {
    return compute_samples::half(static_cast<uint16_t>(0x0400));
  };
  static compute_samples::half max() {
    return compute_samples::half(static_cast<uint16_t>(0x7bff));
  };
  static compute_samples::half infinity() {
    return compute_samples::half(static_cast<uint16_t>(0x7c00));
  };
  static compute_samples::half lowest() {
    return compute_samples::half(static_cast<uint16_t>(0xfbff)); // -max
  };
  static compute_samples::half denorm_min() {
    return compute_samples::half(static_cast<uint16_t>(0x0001));
  };
  // specific quiet and signaling NaN encoding is architecture dependent
  static compute_samples::half quiet_NaN() {
    return compute_samples::half(static_cast<uint16_t>(0x7c01));
  };
  static compute_samples::half signaling_NaN() {
    // in F16C x86 extension f32 sNaN is converted to f16 qNaN
    // ref: Intel 64 and IA-32 Architectures Software Developer's Manual
    // Volume 1, order number 325462-069US, table 14-9
    return quiet_NaN();
  };
};
template <> struct is_unsigned<compute_samples::half> {
  static const bool value = false;
};

} // namespace std
#endif
