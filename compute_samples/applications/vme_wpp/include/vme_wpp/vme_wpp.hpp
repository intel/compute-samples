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

#ifndef COMPUTE_SAMPLES_VME_WPP_HPP
#define COMPUTE_SAMPLES_VME_WPP_HPP

#include <vector>

#include <boost/log/sources/logger.hpp>
namespace src = boost::log::sources;

#include <boost/compute/core.hpp>
namespace compute = boost::compute;

#include "application/application.hpp"
#include "yuv_utils/yuv_utils.hpp"

namespace compute_samples {
class VmeWppApplication : public Application {
private:
  void run_implementation(std::vector<std::string> &command_line,
                          src::logger &logger) override;
  struct Arguments {
    bool output_bmp = false;
    std::string input_yuv_path = "";
    std::string output_yuv_path = "";
    size_t qp = 0;
    size_t width = 0;
    size_t height = 0;
    size_t frames = 0;
    bool help = false;
  };

  void
  run_vme_wpp(const VmeWppApplication::Arguments &args, compute::device &device,
              compute::context &context, compute::command_queue &queue,
              compute::kernel &ds_kernel, compute::kernel &wpp_n_kernel,
              compute::kernel &wpp_kernel, Capture &capture,
              PlanarImage &planar_image, compute::image2d &src_image,
              compute::image2d &ref_image, compute::image2d &src_2x_image,
              compute::image2d &ref_2x_image, compute::image2d &src_4x_image,
              compute::image2d &ref_4x_image, compute::image2d &src_8x_image,
              compute::image2d &ref_8x_image, size_t frame_idx,
              src::logger &logger) const;
  Arguments parse_command_line(const std::vector<std::string> &command_line);
};
} // namespace compute_samples

#endif
