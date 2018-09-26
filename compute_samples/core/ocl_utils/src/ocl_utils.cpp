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
compute::program build_program(const compute::context &context,
                               const std::string &file,
                               const std::string &options) {
  compute::program program =
      compute::program::create_with_source_file(file, context);
  try {
    program.build(options);
  } catch (compute::opencl_error &) {
    src::logger logger;
    BOOST_LOG(logger) << "OpenCL Program Build Error!";
    BOOST_LOG(logger) << "OpenCL Program Build Log is:\n"
                      << program.build_log();
    throw;
  }
  return program;
}
} // namespace compute_samples
