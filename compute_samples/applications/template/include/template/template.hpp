/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_TEMPLATE_HPP
#define COMPUTE_SAMPLES_TEMPLATE_HPP

#include <vector>

#include <boost/compute/core.hpp>

#include "application/application.hpp"

namespace compute_samples {
class TemplateApplication : public Application {
private:
  Status run_implementation(std::vector<std::string> &command_line) override;
  struct Arguments {
    bool help = false;
  };
  Arguments
  parse_command_line(const std::vector<std::string> &command_line) const;
};
} // namespace compute_samples

#endif
