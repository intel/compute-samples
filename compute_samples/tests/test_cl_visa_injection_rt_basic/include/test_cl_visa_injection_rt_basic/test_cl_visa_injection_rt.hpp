/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_TEST_CL_VISA_INJECTION_RT_HPP
#define COMPUTE_SAMPLES_TEST_CL_VISA_INJECTION_RT_HPP

#include <vector>

#include <boost/compute/core.hpp>
namespace compute = boost::compute;

namespace {

#define CL_DEVICE_SUB_GROUP_SIZES_INTEL 0x4108

bool check_supported_subgroup_size(size_t sz) {
  compute::device device = compute::system::default_device();
  std::vector<size_t> intel_sg_sizes =
      device.get_info<std::vector<size_t>>(CL_DEVICE_SUB_GROUP_SIZES_INTEL);

  for (const auto s : intel_sg_sizes) {
    if (sz == s) {
      return true;
    }
  }

  return false;
}

} // namespace

#endif
