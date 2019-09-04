/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_USM_QUERIES_HPP
#define COMPUTE_SAMPLES_USM_QUERIES_HPP

#include <vector>

#include "application/application.hpp"
#include "ocl_entrypoints/cl_intel_unified_shared_memory.h"

namespace compute_samples {
class UsmQueriesApplication : public Application {
private:
  Status run_implementation(std::vector<std::string> &command_line) override;
  struct Arguments {
    bool help = false;
  };
  Arguments
  parse_command_line(const std::vector<std::string> &command_line) const;
};
std::string to_string(const cl_unified_shared_memory_capabilities_intel &x);
} // namespace compute_samples

#endif
