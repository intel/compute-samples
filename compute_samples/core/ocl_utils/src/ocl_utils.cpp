/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ocl_utils/ocl_utils.hpp"
#include "utils/utils.hpp"
#include "logging/logging.hpp"

namespace compute = boost::compute;

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

compute::program create_with_source_file(const std::string &file,
                                         const compute::context &context) {
  std::string text_file = load_text_file(file);
  return compute::program::create_with_source(text_file, context);
}

compute::program build_program(const compute::context &context,
                               const std::string &file,
                               const std::string &options) {
  LOG_ENTER_FUNCTION
  LOG_DEBUG << "Program file: " << file;
  LOG_DEBUG << "Build options: " << options;

  compute::program program = create_with_source_file(file, context);
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
  if (error != 0) {
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
template <> std::string to_cl_c_string<cl_long>() { return "long"; }
template <> std::string to_cl_c_string<cl_long2>() { return "long2"; }
template <> std::string to_cl_c_string<cl_long4>() { return "long4"; }
template <> std::string to_cl_c_string<cl_long8>() { return "long8"; }
template <> std::string to_cl_c_string<cl_long16>() { return "long16"; }
template <> std::string to_cl_c_string<cl_ulong>() { return "ulong"; }
template <> std::string to_cl_c_string<cl_ulong2>() { return "ulong2"; }
template <> std::string to_cl_c_string<cl_ulong4>() { return "ulong4"; }
template <> std::string to_cl_c_string<cl_ulong8>() { return "ulong8"; }
template <> std::string to_cl_c_string<cl_ulong16>() { return "ulong16"; }

template <> std::string to_cl_c_string<compute_samples::cl_char3>() {
  return "char3";
}
template <> std::string to_cl_c_string<compute_samples::cl_uchar3>() {
  return "uchar3";
}
template <> std::string to_cl_c_string<compute_samples::cl_short3>() {
  return "short3";
}
template <> std::string to_cl_c_string<compute_samples::cl_ushort3>() {
  return "ushort3";
}
template <> std::string to_cl_c_string<compute_samples::cl_int3>() {
  return "int3";
}
template <> std::string to_cl_c_string<compute_samples::cl_uint3>() {
  return "uint3";
}
template <> std::string to_cl_c_string<compute_samples::cl_float3>() {
  return "float3";
}
template <> std::string to_cl_c_string<compute_samples::cl_long3>() {
  return "long3";
}
template <> std::string to_cl_c_string<compute_samples::cl_ulong3>() {
  return "ulong3";
}
bool operator==(const compute_samples::cl_char3 &lhs,
                const compute_samples::cl_char3 &rhs) {
  return compute_samples::compare_cl_vectors3(lhs, rhs);
}
bool operator==(const compute_samples::cl_uchar3 &lhs,
                const compute_samples::cl_uchar3 &rhs) {
  return compute_samples::compare_cl_vectors3(lhs, rhs);
}
bool operator==(const compute_samples::cl_short3 &lhs,
                const compute_samples::cl_short3 &rhs) {
  return compute_samples::compare_cl_vectors3(lhs, rhs);
}
bool operator==(const compute_samples::cl_ushort3 &lhs,
                const compute_samples::cl_ushort3 &rhs) {
  return compute_samples::compare_cl_vectors3(lhs, rhs);
}
bool operator==(const compute_samples::cl_int3 &lhs,
                const compute_samples::cl_int3 &rhs) {
  return compute_samples::compare_cl_vectors3(lhs, rhs);
}
bool operator==(const compute_samples::cl_uint3 &lhs,
                const compute_samples::cl_uint3 &rhs) {
  return compute_samples::compare_cl_vectors3(lhs, rhs);
}
bool operator==(const compute_samples::cl_float3 &lhs,
                const compute_samples::cl_float3 &rhs) {
  return compute_samples::compare_cl_vectors3(lhs, rhs);
}
bool operator==(const compute_samples::cl_long3 &lhs,
                const compute_samples::cl_long3 &rhs) {
  return compute_samples::compare_cl_vectors3(lhs, rhs);
}
bool operator==(const compute_samples::cl_ulong3 &lhs,
                const compute_samples::cl_ulong3 &rhs) {
  return compute_samples::compare_cl_vectors3(lhs, rhs);
}

std::ostream &operator<<(std::ostream &os, const compute_samples::cl_char3 &x) {
  return os << compute_samples::cl_vector3_to_string(x);
}
std::ostream &operator<<(std::ostream &os,
                         const compute_samples::cl_uchar3 &x) {
  return os << compute_samples::cl_vector3_to_string(x);
}
std::ostream &operator<<(std::ostream &os,
                         const compute_samples::cl_short3 &x) {
  return os << compute_samples::cl_vector3_to_string(x);
}
std::ostream &operator<<(std::ostream &os,
                         const compute_samples::cl_ushort3 &x) {
  return os << compute_samples::cl_vector3_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const compute_samples::cl_int3 &x) {
  return os << compute_samples::cl_vector3_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const compute_samples::cl_uint3 &x) {
  return os << compute_samples::cl_vector3_to_string(x);
}
std::ostream &operator<<(std::ostream &os,
                         const compute_samples::cl_float3 &x) {
  return os << compute_samples::cl_vector3_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const compute_samples::cl_long3 &x) {
  return os << compute_samples::cl_vector3_to_string(x);
}
std::ostream &operator<<(std::ostream &os,
                         const compute_samples::cl_ulong3 &x) {
  return os << compute_samples::cl_vector3_to_string(x);
}
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
bool operator==(const cl_uint2 &lhs, const cl_uint2 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_uint4 &lhs, const cl_uint4 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_uint8 &lhs, const cl_uint8 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_uint16 &lhs, const cl_uint16 &rhs) {
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
bool operator==(const cl_long2 &lhs, const cl_long2 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_long4 &lhs, const cl_long4 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_long8 &lhs, const cl_long8 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_long16 &lhs, const cl_long16 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_ulong2 &lhs, const cl_ulong2 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_ulong4 &lhs, const cl_ulong4 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_ulong8 &lhs, const cl_ulong8 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_ulong16 &lhs, const cl_ulong16 &rhs) {
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
std::ostream &operator<<(std::ostream &os, const cl_uint2 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_uint4 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_uint8 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_uint16 &x) {
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
std::ostream &operator<<(std::ostream &os, const cl_long2 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_long4 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_long8 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_long16 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_ulong2 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_ulong4 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_ulong8 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_ulong16 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
