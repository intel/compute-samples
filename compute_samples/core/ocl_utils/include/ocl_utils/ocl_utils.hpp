/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_OCL_UTILS_HPP
#define COMPUTE_SAMPLES_OCL_UTILS_HPP

#include <vector>

#include <boost/compute/core.hpp>

namespace compute_samples {
boost::compute::program
build_program(const boost::compute::context &context, const std::string &file,
              const std::string &options = std::string());
boost::compute::program build_program_il(const boost::compute::context &context,
                                         const std::string &file,
                                         const std::string &options = "");

template <typename T> std::string to_cl_c_string();

template <typename T> struct cl_scalar_type {};
template <> struct cl_scalar_type<cl_int16> { using type = cl_int; };
template <> struct cl_scalar_type<cl_uint16> { using type = cl_uint; };
template <> struct cl_scalar_type<cl_int8> { using type = cl_int; };
template <> struct cl_scalar_type<cl_uint8> { using type = cl_uint; };
template <> struct cl_scalar_type<cl_int4> { using type = cl_int; };
template <> struct cl_scalar_type<cl_uint4> { using type = cl_uint; };
template <> struct cl_scalar_type<cl_int2> { using type = cl_int; };
template <> struct cl_scalar_type<cl_uint2> { using type = cl_uint; };
template <> struct cl_scalar_type<cl_int> { using type = cl_int; };
template <> struct cl_scalar_type<cl_uint> { using type = cl_uint; };
template <> struct cl_scalar_type<cl_short16> { using type = cl_short; };
template <> struct cl_scalar_type<cl_ushort16> { using type = cl_ushort; };
template <> struct cl_scalar_type<cl_short8> { using type = cl_short; };
template <> struct cl_scalar_type<cl_ushort8> { using type = cl_ushort; };
template <> struct cl_scalar_type<cl_short4> { using type = cl_short; };
template <> struct cl_scalar_type<cl_ushort4> { using type = cl_ushort; };
template <> struct cl_scalar_type<cl_short2> { using type = cl_short; };
template <> struct cl_scalar_type<cl_ushort2> { using type = cl_ushort; };
template <> struct cl_scalar_type<cl_short> { using type = cl_short; };
template <> struct cl_scalar_type<cl_ushort> { using type = cl_ushort; };
template <> struct cl_scalar_type<cl_char16> { using type = cl_char; };
template <> struct cl_scalar_type<cl_uchar16> { using type = cl_uchar; };
template <> struct cl_scalar_type<cl_char8> { using type = cl_char; };
template <> struct cl_scalar_type<cl_uchar8> { using type = cl_uchar; };
template <> struct cl_scalar_type<cl_char4> { using type = cl_char; };
template <> struct cl_scalar_type<cl_uchar4> { using type = cl_uchar; };
template <> struct cl_scalar_type<cl_char2> { using type = cl_char; };
template <> struct cl_scalar_type<cl_uchar2> { using type = cl_uchar; };
template <> struct cl_scalar_type<cl_char> { using type = cl_char; };
template <> struct cl_scalar_type<cl_uchar> { using type = cl_uchar; };
template <> struct cl_scalar_type<cl_float16> { using type = cl_float; };
template <> struct cl_scalar_type<cl_float8> { using type = cl_float; };
template <> struct cl_scalar_type<cl_float4> { using type = cl_float; };
template <> struct cl_scalar_type<cl_float2> { using type = cl_float; };
template <> struct cl_scalar_type<cl_float> { using type = cl_float; };
template <> struct cl_scalar_type<cl_long> { using type = cl_long; };
template <> struct cl_scalar_type<cl_long2> { using type = cl_long; };
template <> struct cl_scalar_type<cl_long4> { using type = cl_long; };
template <> struct cl_scalar_type<cl_long8> { using type = cl_long; };
template <> struct cl_scalar_type<cl_long16> { using type = cl_long; };
template <> struct cl_scalar_type<cl_ulong> { using type = cl_ulong; };
template <> struct cl_scalar_type<cl_ulong2> { using type = cl_ulong; };
template <> struct cl_scalar_type<cl_ulong4> { using type = cl_ulong; };
template <> struct cl_scalar_type<cl_ulong8> { using type = cl_ulong; };
template <> struct cl_scalar_type<cl_ulong16> { using type = cl_ulong; };

template <typename T> bool compare_cl_vectors(const T &lhs, const T &rhs) {
  const int size = sizeof(T) / sizeof(typename cl_scalar_type<T>::type);
  for (auto i = 0; i < size; ++i) {
    if (lhs.s[i] != rhs.s[i]) {
      return false;
    }
  }
  return true;
}

template <typename T> std::string cl_vector_to_string(const T &x) {
  const int size = sizeof(T) / sizeof(typename cl_scalar_type<T>::type);
  std::stringstream ss;
  ss << "[";
  for (auto i = 0; i < size; ++i) {
    if (i != 0) {
      ss << ", ";
    }
    // + converts cl_char and cl_uchar to integers instead of ASCII characters
    ss << +x.s[i];
  }
  ss << "]";
  return ss.str();
}

} // namespace compute_samples

// Global namespace
bool operator==(const cl_int2 &lhs, const cl_int2 &rhs);
bool operator==(const cl_int4 &lhs, const cl_int4 &rhs);
bool operator==(const cl_int8 &lhs, const cl_int8 &rhs);
bool operator==(const cl_int16 &lhs, const cl_int16 &rhs);
bool operator==(const cl_uint2 &lhs, const cl_uint2 &rhs);
bool operator==(const cl_uint4 &lhs, const cl_uint4 &rhs);
bool operator==(const cl_uint8 &lhs, const cl_uint8 &rhs);
bool operator==(const cl_uint16 &lhs, const cl_uint16 &rhs);
bool operator==(const cl_short2 &lhs, const cl_short2 &rhs);
bool operator==(const cl_short4 &lhs, const cl_short4 &rhs);
bool operator==(const cl_short8 &lhs, const cl_short8 &rhs);
bool operator==(const cl_short16 &lhs, const cl_short16 &rhs);
bool operator==(const cl_ushort2 &lhs, const cl_ushort2 &rhs);
bool operator==(const cl_ushort4 &lhs, const cl_ushort4 &rhs);
bool operator==(const cl_ushort8 &lhs, const cl_ushort8 &rhs);
bool operator==(const cl_ushort16 &lhs, const cl_ushort16 &rhs);
bool operator==(const cl_char2 &lhs, const cl_char2 &rhs);
bool operator==(const cl_char4 &lhs, const cl_char4 &rhs);
bool operator==(const cl_char8 &lhs, const cl_char8 &rhs);
bool operator==(const cl_char16 &lhs, const cl_char16 &rhs);
bool operator==(const cl_uchar2 &lhs, const cl_uchar2 &rhs);
bool operator==(const cl_uchar4 &lhs, const cl_uchar4 &rhs);
bool operator==(const cl_uchar8 &lhs, const cl_uchar8 &rhs);
bool operator==(const cl_uchar16 &lhs, const cl_uchar16 &rhs);
bool operator==(const cl_long2 &lhs, const cl_long2 &rhs);
bool operator==(const cl_long4 &lhs, const cl_long4 &rhs);
bool operator==(const cl_long8 &lhs, const cl_long8 &rhs);
bool operator==(const cl_long16 &lhs, const cl_long16 &rhs);
bool operator==(const cl_ulong2 &lhs, const cl_ulong2 &rhs);
bool operator==(const cl_ulong4 &lhs, const cl_ulong4 &rhs);
bool operator==(const cl_ulong8 &lhs, const cl_ulong8 &rhs);
bool operator==(const cl_ulong16 &lhs, const cl_ulong16 &rhs);

std::ostream &operator<<(std::ostream &os, const cl_int2 &x);
std::ostream &operator<<(std::ostream &os, const cl_int4 &x);
std::ostream &operator<<(std::ostream &os, const cl_int8 &x);
std::ostream &operator<<(std::ostream &os, const cl_int16 &x);
std::ostream &operator<<(std::ostream &os, const cl_uint2 &x);
std::ostream &operator<<(std::ostream &os, const cl_uint4 &x);
std::ostream &operator<<(std::ostream &os, const cl_uint8 &x);
std::ostream &operator<<(std::ostream &os, const cl_uint16 &x);
std::ostream &operator<<(std::ostream &os, const cl_short2 &x);
std::ostream &operator<<(std::ostream &os, const cl_short4 &x);
std::ostream &operator<<(std::ostream &os, const cl_short8 &x);
std::ostream &operator<<(std::ostream &os, const cl_short16 &x);
std::ostream &operator<<(std::ostream &os, const cl_ushort2 &x);
std::ostream &operator<<(std::ostream &os, const cl_ushort4 &x);
std::ostream &operator<<(std::ostream &os, const cl_ushort8 &x);
std::ostream &operator<<(std::ostream &os, const cl_ushort16 &x);
std::ostream &operator<<(std::ostream &os, const cl_char2 &x);
std::ostream &operator<<(std::ostream &os, const cl_char4 &x);
std::ostream &operator<<(std::ostream &os, const cl_char8 &x);
std::ostream &operator<<(std::ostream &os, const cl_char16 &x);
std::ostream &operator<<(std::ostream &os, const cl_uchar2 &x);
std::ostream &operator<<(std::ostream &os, const cl_uchar4 &x);
std::ostream &operator<<(std::ostream &os, const cl_uchar8 &x);
std::ostream &operator<<(std::ostream &os, const cl_uchar16 &x);
std::ostream &operator<<(std::ostream &os, const cl_long2 &x);
std::ostream &operator<<(std::ostream &os, const cl_long4 &x);
std::ostream &operator<<(std::ostream &os, const cl_long8 &x);
std::ostream &operator<<(std::ostream &os, const cl_long16 &x);
std::ostream &operator<<(std::ostream &os, const cl_ulong2 &x);
std::ostream &operator<<(std::ostream &os, const cl_ulong4 &x);
std::ostream &operator<<(std::ostream &os, const cl_ulong8 &x);
std::ostream &operator<<(std::ostream &os, const cl_ulong16 &x);

#endif
