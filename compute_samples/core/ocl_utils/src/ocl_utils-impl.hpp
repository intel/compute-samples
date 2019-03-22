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

#ifndef COMPUTE_SAMPLES_OCL_UTILS_IMPL_HPP
#define COMPUTE_SAMPLES_OCL_UTILS_IMPL_HPP

#include "ocl_utils/ocl_utils.hpp"

namespace compute_samples {
template <> struct cl_scalar_type<cl_int8> { using type = cl_int; };
template <> struct cl_scalar_type<cl_uint8> { using type = cl_uint; };
template <> struct cl_scalar_type<cl_int4> { using type = cl_int; };
template <> struct cl_scalar_type<cl_uint4> { using type = cl_uint; };
template <> struct cl_scalar_type<cl_int2> { using type = cl_int; };
template <> struct cl_scalar_type<cl_uint2> { using type = cl_uint; };
template <> struct cl_scalar_type<cl_int> { using type = cl_int; };
template <> struct cl_scalar_type<cl_uint> { using type = cl_uint; };
template <> struct cl_scalar_type<cl_short8> { using type = cl_short; };
template <> struct cl_scalar_type<cl_ushort8> { using type = cl_ushort; };
template <> struct cl_scalar_type<cl_short4> { using type = cl_short; };
template <> struct cl_scalar_type<cl_ushort4> { using type = cl_ushort; };
template <> struct cl_scalar_type<cl_short2> { using type = cl_short; };
template <> struct cl_scalar_type<cl_ushort2> { using type = cl_ushort; };
template <> struct cl_scalar_type<cl_short> { using type = cl_short; };
template <> struct cl_scalar_type<cl_ushort> { using type = cl_ushort; };
template <> struct cl_scalar_type<cl_char8> { using type = cl_char; };
template <> struct cl_scalar_type<cl_uchar8> { using type = cl_uchar; };
template <> struct cl_scalar_type<cl_char4> { using type = cl_char; };
template <> struct cl_scalar_type<cl_uchar4> { using type = cl_uchar; };
template <> struct cl_scalar_type<cl_char2> { using type = cl_char; };
template <> struct cl_scalar_type<cl_uchar2> { using type = cl_uchar; };
template <> struct cl_scalar_type<cl_char> { using type = cl_char; };
template <> struct cl_scalar_type<cl_uchar> { using type = cl_uchar; };
template <> struct cl_scalar_type<cl_float8> { using type = cl_float; };
template <> struct cl_scalar_type<cl_float4> { using type = cl_float; };
template <> struct cl_scalar_type<cl_float2> { using type = cl_float; };
template <> struct cl_scalar_type<cl_float> { using type = cl_float; };

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
    ss << x.s[i];
  }
  ss << "]";
  return ss.str();
}

} // namespace compute_samples

#endif
