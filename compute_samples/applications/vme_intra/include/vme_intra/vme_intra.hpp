/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_VME_INTRA_HPP
#define COMPUTE_SAMPLES_VME_INTRA_HPP

#include <vector>

#include <boost/compute/core.hpp>
namespace compute = boost::compute;

#include "application/application.hpp"
#include "yuv_utils/yuv_utils.hpp"

namespace compute_samples {
class VmeIntraApplication : public Application {
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

  void
  run_vme_intra(const VmeIntraApplication::Arguments &args,
                compute::context &context, compute::command_queue &queue,
                compute::kernel &ds_kernel, compute::kernel &hme_n_kernel,
                compute::kernel &intra_kernel, Capture &capture,
                PlanarImage &planar_image, compute::image2d &src_image,
                compute::image2d &ref_image, compute::image2d &src_2x_image,
                compute::image2d &ref_2x_image, compute::image2d &src_4x_image,
                compute::image2d &ref_4x_image, compute::image2d &src_8x_image,
                compute::image2d &ref_8x_image, int frame_idx) const;
  void write_results_to_file(const cl_ulong *intra_modes,
                             const cl_uchar *intra_shapes,
                             const cl_ushort *intra_residuals,
                             const cl_ushort *inter_best_residuals,
                             uint32_t width, uint32_t height,
                             uint32_t frame_idx) const;
  Arguments parse_command_line(const std::vector<std::string> &command_line);
};
} // namespace compute_samples

#endif
