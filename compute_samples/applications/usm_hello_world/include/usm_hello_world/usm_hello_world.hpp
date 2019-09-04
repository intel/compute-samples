/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_USM_HELLO_WORLD_HPP
#define COMPUTE_SAMPLES_USM_HELLO_WORLD_HPP

#include <vector>

#include <boost/compute/core.hpp>
#include <boost/compute/intel/usm.hpp>
namespace compute = boost::compute;

#include "application/application.hpp"

namespace compute_samples {

class UsmHelloWorldApplication : public Application {
private:
  Status run_implementation(std::vector<std::string> &command_line) override;
  struct Arguments {
    bool help = false;
    compute::usm_type type = compute::usm_type::host;
    size_t size = 0;
  };
  Arguments
  parse_command_line(const std::vector<std::string> &command_line) const;
};

std::vector<cl_uint> copy_buffer(const std::vector<cl_uint> &input,
                                 const compute::usm_type type);

} // namespace compute_samples

#endif
