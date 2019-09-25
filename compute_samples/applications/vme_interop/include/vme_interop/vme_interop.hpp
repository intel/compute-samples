/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_VME_INTEROP_HPP
#define COMPUTE_SAMPLES_VME_INTEROP_HPP

#include <string>
#include <vector>

#include <boost/compute/core.hpp>
namespace compute = boost::compute;

#include "application/application.hpp"

namespace compute_samples {
class VmeInteropApplication : public Application {
public:
  static const char *vme_extension_msg_;
  static const char *vaapi_extension_msg_;

  struct Arguments {
    bool output_bmp = false;
    std::string input_yuv_path = "";
    std::string output_yuv_path = "";
    int width = 0;
    int height = 0;
    int frames = 0;
    bool help = false;
  };

private:
  Status run_implementation(std::vector<std::string> &command_line) override;

  Arguments parse_command_line(const std::vector<std::string> &command_line);
  void run_os_specific_implementation(const Arguments &,
                                      const compute::device &) const;
};
} // namespace compute_samples

#endif
