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

#ifndef COMPUTE_SAMPLES_SUBGROUPS_VISUALIZATION_HPP
#define COMPUTE_SAMPLES_SUBGROUPS_VISUALIZATION_HPP

#include <vector>

#include <boost/log/sources/logger.hpp>
namespace src = boost::log::sources;

#include <boost/compute/core.hpp>
namespace compute = boost::compute;

#include "application/application.hpp"

namespace compute_samples {

class SubgroupsVisualizationApplication : public Application {
private:
  void run_implementation(std::vector<std::string> &command_line,
                          src::logger &logger) override;
  struct Arguments {
    std::string kernel_path = "";
    std::string output = "";
    size_t global_size = 0;
    size_t local_size = 0;
    bool help = false;
  };
  Arguments
  parse_command_line(const std::vector<std::string> &command_line) const;
  void run_subgroups_visualization(const Arguments &args,
                                   compute::context &context,
                                   compute::command_queue &queue,
                                   src::logger &logger) const;
};

} // namespace compute_samples

#endif
