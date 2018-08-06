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

#ifndef COMPUTE_SAMPLES_VME_INTERLACED_HPP
#define COMPUTE_SAMPLES_VME_INTERLACED_HPP

#include <vector>

#include <boost/log/sources/logger.hpp>
namespace src = boost::log::sources;

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
  void run_implementation(std::vector<std::string> &command_line,
                          src::logger &logger) override;
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
      compute::kernel &kernel, Capture &capture, PlanarImage &planar_image,
      PlanarImage &top_planar_image, PlanarImage &bot_planar_image,
      compute::image2d &src_image, compute::image2d &ref_image, int frame_idx,
      src::logger &logger) const;
  void run_vme_interlaced_split(const VmeInterlacedApplication::Arguments &args,
                                compute::context &context,
                                compute::command_queue &queue,
                                compute::kernel &kernel, Capture &capture,
                                PlanarImage &field_planar_image,
                                compute::image2d &src_image,
                                compute::image2d &ref_image, int polarity,
                                int frame_idx, src::logger &logger) const;
  void run_vme_interlaced(
      const VmeInterlacedApplication::Arguments &args,
      compute::context &context, compute::command_queue &queue,
      compute::kernel &kernel, Capture &capture, PlanarImage &planar_image,
      compute::image2d &src_image, compute::image2d &ref_image,
      au::PageAlignedVector<cl_short2> &field_mvs,
      au::PageAlignedVector<cl_uchar2> &field_shapes,
      au::PageAlignedVector<cl_ushort> &residuals,
      au::PageAlignedVector<cl_short2> &predictors, int width, int mb_count,
      int mv_count, uint32_t iterations, uint8_t interlaced, int polarity,
      int frame_idx, Timer &timer, src::logger &logger) const;
  void get_field_capture_samples(Capture *capture,
                                 PlanarImage *top_planar_image,
                                 PlanarImage *bot_planar_image, int frame_idx,
                                 src::logger *logger) const;
  Arguments parse_command_line(const std::vector<std::string> &command_line);
};
} // namespace compute_samples

#endif
