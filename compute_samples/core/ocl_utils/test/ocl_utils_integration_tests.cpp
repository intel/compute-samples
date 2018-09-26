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

  const std::string cl_file = "kernel.cl";
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
