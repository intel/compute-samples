/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_SUBGROUPS_VISUALIZATION_HPP
#define COMPUTE_SAMPLES_SUBGROUPS_VISUALIZATION_HPP

#include <vector>

#include <boost/compute/core.hpp>
namespace compute = boost::compute;

#include "application/application.hpp"

namespace compute_samples {

class SubgroupsVisualizationApplication : public Application {
private:
  Status run_implementation(std::vector<std::string> &command_line) override;
  struct Arguments {
    std::string kernel_path = "";
    std::string output = "";
    int global_size = 0;
    int local_size = 0;
    bool help = false;
  };
  Arguments
  parse_command_line(const std::vector<std::string> &command_line) const;
  void run_subgroups_visualization(const Arguments &args,
                                   compute::context &context,
                                   compute::command_queue &queue) const;
};

} // namespace compute_samples

#endif
