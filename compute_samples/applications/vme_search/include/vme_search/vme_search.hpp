/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_VME_SEARCH_HPP
#define COMPUTE_SAMPLES_VME_SEARCH_HPP

#include <vector>

#include <boost/compute/core.hpp>
namespace compute = boost::compute;

#include "application/application.hpp"
#include "yuv_utils/yuv_utils.hpp"

namespace compute_samples {
class VmeSearchApplication : public Application {
private:
  Status run_implementation(std::vector<std::string> &command_line) override;
  struct Arguments {
    bool output_bmp = false;
    std::string sub_test = "";
    std::string input_yuv_path = "";
    std::string output_yuv_path = "";
    int qp = 0;
    int width = 0;
    int height = 0;
    int frames = 0;
    bool help = false;
  };

  void run_vme_search(const VmeSearchApplication::Arguments &args,
                      compute::context &context, compute::command_queue &queue,
                      compute::kernel &kernel, YuvCapture &capture,
                      PlanarImage &src_planar_image,
                      compute::image2d &src_image, compute::image2d &ref_image,
                      int frame_idx) const;
  Arguments parse_command_line(const std::vector<std::string> &command_line);
};
} // namespace compute_samples

#endif
