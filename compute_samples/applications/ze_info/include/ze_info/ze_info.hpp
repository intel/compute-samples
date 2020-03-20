/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_ZE_INFO_ZE_INFO_HPP
#define COMPUTE_SAMPLES_ZE_INFO_ZE_INFO_HPP

#include <vector>
#include "application/application.hpp"

namespace compute_samples {
class ZeInfoApplication : public Application {
private:
  Status run_implementation(std::vector<std::string> &command_line) override;
  struct Arguments {
    bool help = false;
    std::string output = "";
  };
  Arguments
  parse_command_line(const std::vector<std::string> &command_line) const;
};
} // namespace compute_samples

#endif
