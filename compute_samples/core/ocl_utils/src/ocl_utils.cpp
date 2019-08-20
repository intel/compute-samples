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

#include "ocl_utils/ocl_utils.hpp"
#include "utils/utils.hpp"
#include "logging/logging.hpp"

namespace compute_samples {
void try_build(compute::program &program, const std::string &options) {
  LOG_ENTER_FUNCTION
  LOG_DEBUG << "Build options: " << options;

  try {
    program.build(options);
  } catch (compute::opencl_error &) {
    LOG_ERROR << "OpenCL Program Build Error!";
    LOG_ERROR << "OpenCL Program Build Log is:\n" << program.build_log();
    LOG_TRACE << "Throwing exception from try_build";
    throw;
  }

  LOG_DEBUG << "Program successfully built";
  LOG_EXIT_FUNCTION
}

compute::program build_program(const compute::context &context,
                               const std::string &file,
                               const std::string &options) {
  LOG_ENTER_FUNCTION
  LOG_DEBUG << "Program file: " << file;
  LOG_DEBUG << "Build options: " << options;

  compute::program program =
      compute::program::create_with_source_file(file, context);
  LOG_DEBUG << "Program successfully created with source file";
  try_build(program, options);

  LOG_EXIT_FUNCTION
  return program;
}

compute::program create_with_il_file(const std::string &file,
                                     const compute::context &context) {
  std::vector<uint8_t> binary_spv_file = load_binary_file(file);

  cl_int error = 0;
  cl_program clprogram = clCreateProgramWithIL(context, binary_spv_file.data(),
                                               binary_spv_file.size(), &error);
  if (error) {
    BOOST_THROW_EXCEPTION(compute::opencl_error(error));
  }

  return compute::program(clprogram, true);
}

compute::program build_program_il(const compute::context &context,
                                  const std::string &file,
                                  const std::string &options) {
  LOG_ENTER_FUNCTION
  LOG_DEBUG << "Program file: " << file;
  LOG_DEBUG << "Build options: " << options;

  compute::program program = create_with_il_file(file, context);
  LOG_DEBUG << "Program successfully created with il file";
  try_build(program, options);

  LOG_EXIT_FUNCTION
  return program;
}

template <> std::string to_cl_c_string<cl_int>() { return "int"; }
template <> std::string to_cl_c_string<cl_int2>() { return "int2"; }
template <> std::string to_cl_c_string<cl_int4>() { return "int4"; }
template <> std::string to_cl_c_string<cl_int8>() { return "int8"; }
template <> std::string to_cl_c_string<cl_int16>() { return "int16"; }
template <> std::string to_cl_c_string<cl_uint>() { return "uint"; }
template <> std::string to_cl_c_string<cl_uint2>() { return "uint2"; }
template <> std::string to_cl_c_string<cl_uint4>() { return "uint4"; }
template <> std::string to_cl_c_string<cl_uint8>() { return "uint8"; }
template <> std::string to_cl_c_string<cl_uint16>() { return "uint16"; }
template <> std::string to_cl_c_string<cl_short>() { return "short"; }
template <> std::string to_cl_c_string<cl_short2>() { return "short2"; }
template <> std::string to_cl_c_string<cl_short4>() { return "short4"; }
template <> std::string to_cl_c_string<cl_short8>() { return "short8"; }
template <> std::string to_cl_c_string<cl_short16>() { return "short16"; }
template <> std::string to_cl_c_string<cl_ushort>() { return "ushort"; }
template <> std::string to_cl_c_string<cl_ushort2>() { return "ushort2"; }
template <> std::string to_cl_c_string<cl_ushort4>() { return "ushort4"; }
template <> std::string to_cl_c_string<cl_ushort8>() { return "ushort8"; }
template <> std::string to_cl_c_string<cl_ushort16>() { return "ushort16"; }
template <> std::string to_cl_c_string<cl_char>() { return "char"; }
template <> std::string to_cl_c_string<cl_char2>() { return "char2"; }
template <> std::string to_cl_c_string<cl_char4>() { return "char4"; }
template <> std::string to_cl_c_string<cl_char8>() { return "char8"; }
template <> std::string to_cl_c_string<cl_char16>() { return "char16"; }
template <> std::string to_cl_c_string<cl_uchar>() { return "uchar"; }
template <> std::string to_cl_c_string<cl_uchar2>() { return "uchar2"; }
template <> std::string to_cl_c_string<cl_uchar4>() { return "uchar4"; }
template <> std::string to_cl_c_string<cl_uchar8>() { return "uchar8"; }
template <> std::string to_cl_c_string<cl_uchar16>() { return "uchar16"; }
template <> std::string to_cl_c_string<cl_float>() { return "float"; }
template <> std::string to_cl_c_string<cl_float2>() { return "float2"; }
template <> std::string to_cl_c_string<cl_float4>() { return "float4"; }
template <> std::string to_cl_c_string<cl_float8>() { return "float8"; }
template <> std::string to_cl_c_string<cl_float16>() { return "float16"; }
template <> std::string to_cl_c_string<cl_double>() { return "double"; }
template <> std::string to_cl_c_string<cl_double2>() { return "double2"; }
template <> std::string to_cl_c_string<cl_double4>() { return "double4"; }
template <> std::string to_cl_c_string<cl_double8>() { return "double8"; }
template <> std::string to_cl_c_string<cl_double16>() { return "double16"; }

} // namespace compute_samples

bool operator==(const cl_int2 &lhs, const cl_int2 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_int4 &lhs, const cl_int4 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_int8 &lhs, const cl_int8 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_int16 &lhs, const cl_int16 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_short2 &lhs, const cl_short2 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_short4 &lhs, const cl_short4 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_short8 &lhs, const cl_short8 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_short16 &lhs, const cl_short16 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_ushort2 &lhs, const cl_ushort2 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_ushort4 &lhs, const cl_ushort4 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_ushort8 &lhs, const cl_ushort8 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_ushort16 &lhs, const cl_ushort16 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_char2 &lhs, const cl_char2 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_char4 &lhs, const cl_char4 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_char8 &lhs, const cl_char8 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_char16 &lhs, const cl_char16 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_uchar2 &lhs, const cl_uchar2 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_uchar4 &lhs, const cl_uchar4 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_uchar8 &lhs, const cl_uchar8 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_uchar16 &lhs, const cl_uchar16 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}

std::ostream &operator<<(std::ostream &os, const cl_int2 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_int4 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_int8 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_int16 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_short2 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_short4 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_short8 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_short16 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_ushort2 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_ushort4 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_ushort8 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_ushort16 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_char2 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_char4 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_char8 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_char16 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_uchar2 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_uchar4 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_uchar8 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_uchar16 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
