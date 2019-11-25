/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_OFFLINE_COMPILER_HPP
#define COMPUTE_SAMPLES_OFFLINE_COMPILER_HPP

#include <string>
#include <vector>

namespace compute_samples {

std::string generate_spirv(const std::string &path,
                           const std::string &build_options = std::string());
std::vector<uint8_t>
generate_spirv_from_source(const std::string &source,
                           const std::string &build_options = std::string());

} // namespace compute_samples

#endif
