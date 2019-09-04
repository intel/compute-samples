/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_SUBGROUPS_IMAGECOPY_HPP
#define COMPUTE_SAMPLES_SUBGROUPS_IMAGECOPY_HPP

#include <vector>

#include <boost/compute/core.hpp>
namespace compute = boost::compute;

#include "application/application.hpp"

namespace compute_samples {

class SubgroupsImageCopyApplication : public Application {
private:
  Status run_implementation(std::vector<std::string> &command_line) override;
  struct Arguments {
    std::string kernel_path = "";
    std::string input = "";
    std::string output = "";
    int iterations = 1000;
    bool help = false;
  };
  Arguments
  parse_command_line(const std::vector<std::string> &command_line) const;
  void run_subgroups_imagecopy(const Arguments &args, compute::context &context,
                               compute::command_queue &queue) const;
};

} // namespace compute_samples

#endif
