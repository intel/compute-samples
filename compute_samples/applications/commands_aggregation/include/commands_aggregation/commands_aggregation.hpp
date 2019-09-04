/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_COMMANDS_AGGREGATION_HPP
#define COMPUTE_SAMPLES_COMMANDS_AGGREGATION_HPP

#include <vector>

#include <boost/compute/core.hpp>
namespace compute = boost::compute;

#include "application/application.hpp"

namespace compute_samples {
class CommandsAggregationApplication : public Application {
public:
  std::vector<uint32_t>
  run_workloads_out_of_order(const int global_work_size) const;
  std::vector<uint32_t>
  run_workloads_in_order(const int global_work_size) const;

private:
  Status run_implementation(std::vector<std::string> &command_line) override;
  struct Arguments {
    bool help = false;
    bool in_order = false;
    int global_work_size = 0;
  };
  Arguments
  parse_command_line(const std::vector<std::string> &command_line) const;
};
} // namespace compute_samples

#endif
