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
#include "gtest/gtest.h"

#include <fstream>

#include <boost/compute/utility/source.hpp>

namespace cs = compute_samples;

class BuildProgram : public testing::Test {
protected:
  virtual void SetUp() {
    device = compute::system::default_device();
    context = compute::context(device);
  }

  virtual void TearDown() { std::remove(cl_file.c_str()); }

  void write_source_to_file(const std::string &source,
                            const std::string &file) {
    std::ofstream ofs(file);
    ofs << source;
    ofs.close();
  }

  void write_binary_to_file(void *source, size_t size,
                            const std::string &file) {
    typedef std::basic_ofstream<uint8_t, std::char_traits<uint8_t>> uofstream;
    uofstream ofs(file, std::ios::binary);
    ofs.write(static_cast<uint8_t *>(source), size);
    ofs.close();
  }

  const std::string cl_file = "kernel.cl";
  const std::string spv_file = "kernel.spv";
  compute::device device;
  compute::context context;
};

TEST_F(BuildProgram, ValidProgram) {
  const char source[] =
      BOOST_COMPUTE_STRINGIZE_SOURCE(kernel void my_kernel(){});
  write_source_to_file(source, cl_file);
  EXPECT_NE(compute::program(), cs::build_program(context, cl_file));
}

TEST_F(BuildProgram, InvalidProgram) {
  const char source[] =
      BOOST_COMPUTE_STRINGIZE_SOURCE(kernel invalid_type my_kernel(){});
  write_source_to_file(source, cl_file);
  EXPECT_THROW(cs::build_program(context, cl_file), compute::opencl_error);
}

TEST_F(BuildProgram, BuildOptions) {
  const char source[] =
      BOOST_COMPUTE_STRINGIZE_SOURCE(kernel MY_TYPE my_kernel(){});
  write_source_to_file(source, cl_file);
  EXPECT_NE(compute::program(),
            cs::build_program(context, cl_file, "-DMY_TYPE=void"));
}

TEST_F(BuildProgram, ValidProgramSpirV) {
  std::vector<uint8_t> spriv_file_source = {
      0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x0b, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00,
      0x0b, 0x00, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x4f, 0x70, 0x65, 0x6e,
      0x43, 0x4c, 0x2e, 0x73, 0x74, 0x64, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00,
      0x03, 0x00, 0x00, 0x00, 0x70, 0x8e, 0x01, 0x00};
  write_binary_to_file(spriv_file_source.data(), spriv_file_source.size(),
                       spv_file);
  EXPECT_NE(compute::program(),
            cs::build_program_il(context, spv_file, "-cl-std=CL2.0"));
}

TEST_F(BuildProgram, InvalidProgramSpirV) {
  std::vector<uint8_t> spriv_file_source = {
      0x0A, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x0b, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00,
      0x04, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00,
      0x0b, 0x00, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x4f, 0x70, 0x65, 0x6e,
      0x43, 0x4c, 0x2e, 0x73, 0x74, 0x64, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00,
      0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00,
      0x03, 0x00, 0x00, 0x00, 0x70, 0x8e, 0x01, 0x00};
  write_binary_to_file(spriv_file_source.data(), spriv_file_source.size(),
                       spv_file);
  EXPECT_THROW(cs::build_program_il(context, spv_file, "-cl-std=CL2.0"),
               compute::opencl_error);
}
