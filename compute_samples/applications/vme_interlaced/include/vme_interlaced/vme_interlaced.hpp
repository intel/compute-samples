/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_VME_INTERLACED_HPP
#define COMPUTE_SAMPLES_VME_INTERLACED_HPP

#include <vector>

#include <boost/thread.hpp>

#include <boost/compute/core.hpp>
namespace compute = boost::compute;

#include "align_utils/align_utils.hpp"
namespace au = compute_samples::align_utils;

#include "application/application.hpp"
#include "timer/timer.hpp"
#include "yuv_utils/yuv_utils.hpp"

namespace compute_samples {
class VmeInterlacedApplication : public Application {
private:
  Status run_implementation(std::vector<std::string> &command_line) override;
  struct Arguments {
    bool output_bmp = false;
    std::string sub_test = "";
    std::string input_yuv_path = "";
    std::string output_top_yuv_path = "";
    std::string output_bot_yuv_path = "";
    int width = 0;
    int height = 0;
    int frames = 0;
    char native = 1;
    bool help = false;
  };

  void run_vme_interlaced_native(
      const VmeInterlacedApplication::Arguments &args,
      compute::context &context, compute::command_queue &queue,
      compute::kernel &kernel, YuvCapture &capture, PlanarImage &planar_image,
      PlanarImage &top_planar_image, PlanarImage &bot_planar_image,
      compute::image2d &src_image, compute::image2d &ref_image,
      int frame_idx) const;
  void run_vme_interlaced_split(const VmeInterlacedApplication::Arguments &args,
                                compute::context &context,
                                compute::command_queue &queue,
                                compute::kernel &kernel, YuvCapture &capture,
                                PlanarImage &field_planar_image,
                                compute::image2d &src_image,
                                compute::image2d &ref_image, int polarity,
                                int frame_idx) const;
  void run_vme_interlaced(compute::context &context,
                          compute::command_queue &queue,
                          compute::kernel &kernel, compute::image2d &src_image,
                          compute::image2d &ref_image,
                          au::PageAlignedVector<cl_short2> &field_mvs,
                          au::PageAlignedVector<cl_uchar2> &field_shapes,
                          au::PageAlignedVector<cl_ushort> &residuals,
                          au::PageAlignedVector<cl_short2> &predictors,
                          int width, int mb_count, int mv_count,
                          uint32_t iterations, uint8_t interlaced, int polarity,
                          Timer &timer) const;
  void get_field_capture_samples(YuvCapture &capture,
                                 PlanarImage &top_planar_image,
                                 PlanarImage &bot_planar_image,
                                 int frame_idx) const;
  Arguments parse_command_line(const std::vector<std::string> &command_line);
};
} // namespace compute_samples

#endif
