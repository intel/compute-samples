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

#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace src = boost::log::sources;

namespace compute_samples {
void try_build(compute::program &program, const std::string &options) {
  try {
    program.build(options);
  } catch (compute::opencl_error &) {
    src::logger logger;
    BOOST_LOG(logger) << "OpenCL Program Build Error!";
    BOOST_LOG(logger) << "OpenCL Program Build Log is:\n"
                      << program.build_log();
    throw;
  }
}

compute::program build_program(const compute::context &context,
                               const std::string &file,
                               const std::string &options) {
  compute::program program =
      compute::program::create_with_source_file(file, context);
  try_build(program, options);
  return program;
}
compute::program create_with_il_file(const std::string &file,
                                     const compute::context &context) {
  cl_program clprogram;
  std::ifstream sprv_stream(file, std::ios::in | std::ios::binary);

  std::vector<uint8_t> binary_spv_file(
      (std::istreambuf_iterator<char>(sprv_stream)),
      std::istreambuf_iterator<char>());
  cl_int error = 0;
  clprogram = clCreateProgramWithIL(context, binary_spv_file.data(),
                                    binary_spv_file.size(), &error);
  if (error) {
    BOOST_THROW_EXCEPTION(compute::opencl_error(error));
  }

  return compute::program(clprogram, true);
}

compute::program build_program_il(const compute::context &context,
                                  const std::string &file,
                                  const std::string &options) {
  compute::program program = create_with_il_file(file, context);
  try_build(program, options);
  return program;
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
bool operator==(const cl_short2 &lhs, const cl_short2 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_short4 &lhs, const cl_short4 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_short8 &lhs, const cl_short8 &rhs) {
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
bool operator==(const cl_uchar2 &lhs, const cl_uchar2 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_uchar4 &lhs, const cl_uchar4 &rhs) {
  return compute_samples::compare_cl_vectors(lhs, rhs);
}
bool operator==(const cl_uchar8 &lhs, const cl_uchar8 &rhs) {
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
std::ostream &operator<<(std::ostream &os, const cl_short2 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_short4 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_short8 &x) {
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
std::ostream &operator<<(std::ostream &os, const cl_uchar2 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_uchar4 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
std::ostream &operator<<(std::ostream &os, const cl_uchar8 &x) {
  return os << compute_samples::cl_vector_to_string(x);
}
