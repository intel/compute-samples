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

#ifndef COMPUTE_SAMPLES_MEDIAN_FILTER_HPP
#define COMPUTE_SAMPLES_MEDIAN_FILTER_HPP

#include <vector>

#include <boost/compute/core.hpp>
namespace compute = boost::compute;

#include "application/application.hpp"
#include "image/image.hpp"

namespace compute_samples {
class MedianFilterApplication : public Application {
private:
  Status run_implementation(std::vector<std::string> &command_line) override;
  struct Arguments {
    std::string input_image_path = "";
    std::string output_image_path = "";
    std::string kernel_path = "";
    bool help = false;
  };
  Arguments
  parse_command_line(const std::vector<std::string> &command_line) const;
  void run_median_filter(const Arguments &args, compute::context &context,
                         compute::command_queue &queue) const;
  void write_image_to_buffer(const ImagePNG32Bit &image,
                             compute::buffer &buffer,
                             compute::command_queue &queue) const;
  void read_buffer_to_image(compute::buffer &buffer, ImagePNG32Bit &image,
                            compute::command_queue &queue) const;
};
} // namespace compute_samples

#endif
