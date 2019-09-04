/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
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
