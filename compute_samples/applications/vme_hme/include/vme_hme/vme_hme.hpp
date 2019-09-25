/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_VME_HME_HPP
#define COMPUTE_SAMPLES_VME_HME_HPP

#include <vector>

#include <boost/compute/core.hpp>

#include "application/application.hpp"
#include "yuv_utils/yuv_utils.hpp"

namespace compute_samples {
class VmeHmeApplication : public Application {
private:
  Status run_implementation(std::vector<std::string> &command_line) override;
  struct Arguments {
    bool output_bmp = false;
    std::string input_yuv_path = "";
    std::string output_yuv_path = "";
    int qp = 0;
    int width = 0;
    int height = 0;
    int frames = 0;
    bool help = false;
  };

  void run_vme_hme(
      const VmeHmeApplication::Arguments &args,
      boost::compute::context &context, boost::compute::command_queue &queue,
      boost::compute::kernel &ds_kernel, boost::compute::kernel &hme_n_kernel,
      boost::compute::kernel &hme_kernel, YuvCapture &capture,
      PlanarImage &planar_image, boost::compute::image2d &src_image,
      boost::compute::image2d &ref_image, boost::compute::image2d &src_2x_image,
      boost::compute::image2d &ref_2x_image,
      boost::compute::image2d &src_4x_image,
      boost::compute::image2d &ref_4x_image,
      boost::compute::image2d &src_8x_image,
      boost::compute::image2d &ref_8x_image, int frame_idx) const;
  Arguments parse_command_line(const std::vector<std::string> &command_line);
};
} // namespace compute_samples

#endif
