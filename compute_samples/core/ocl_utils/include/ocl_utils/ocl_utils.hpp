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

#ifndef COMPUTE_SAMPLES_OCL_UTILS_HPP
#define COMPUTE_SAMPLES_OCL_UTILS_HPP

#include <vector>

#include <boost/compute/core.hpp>
namespace compute = boost::compute;

#include "image/image.hpp"

namespace compute_samples {
template <typename T> int size_in_bytes(const std::vector<T> &v);
template <typename T> int size_in_bytes(const Image<T> &i);
compute::program build_program(const compute::context &context,
                               const std::string &file,
                               const std::string &options = std::string());
compute::program build_program_il(const compute::context &context,
                                  const std::string &file,
                                  const std::string &options = "");
template <typename T> struct cl_scalar_type {};
template <typename T> bool compare_cl_vectors(const T &lhs, const T &rhs);
template <typename T> std::string cl_vector_to_string(const T &x);
} // namespace compute_samples

// Global namespace
bool operator==(const cl_int2 &lhs, const cl_int2 &rhs);
bool operator==(const cl_int4 &lhs, const cl_int4 &rhs);
bool operator==(const cl_int8 &lhs, const cl_int8 &rhs);
bool operator==(const cl_short2 &lhs, const cl_short2 &rhs);
bool operator==(const cl_short4 &lhs, const cl_short4 &rhs);
bool operator==(const cl_short8 &lhs, const cl_short8 &rhs);
bool operator==(const cl_char2 &lhs, const cl_char2 &rhs);
bool operator==(const cl_char4 &lhs, const cl_char4 &rhs);
bool operator==(const cl_char8 &lhs, const cl_char8 &rhs);
bool operator==(const cl_uchar2 &lhs, const cl_uchar2 &rhs);
bool operator==(const cl_uchar4 &lhs, const cl_uchar4 &rhs);
bool operator==(const cl_uchar8 &lhs, const cl_uchar8 &rhs);

std::ostream &operator<<(std::ostream &os, const cl_int2 &x);
std::ostream &operator<<(std::ostream &os, const cl_int4 &x);
std::ostream &operator<<(std::ostream &os, const cl_int8 &x);
std::ostream &operator<<(std::ostream &os, const cl_short2 &x);
std::ostream &operator<<(std::ostream &os, const cl_short4 &x);
std::ostream &operator<<(std::ostream &os, const cl_short8 &x);
std::ostream &operator<<(std::ostream &os, const cl_char2 &x);
std::ostream &operator<<(std::ostream &os, const cl_char4 &x);
std::ostream &operator<<(std::ostream &os, const cl_char8 &x);
std::ostream &operator<<(std::ostream &os, const cl_uchar2 &x);
std::ostream &operator<<(std::ostream &os, const cl_uchar4 &x);
std::ostream &operator<<(std::ostream &os, const cl_uchar8 &x);

// Include template implementation
#include "ocl_utils-impl.hpp"

#endif
