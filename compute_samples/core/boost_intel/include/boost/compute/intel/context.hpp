/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef BOOST_COMPUTE_INTEL_CONTEXT_HPP
#define BOOST_COMPUTE_INTEL_CONTEXT_HPP

#include <boost/compute/context.hpp>

namespace boost {
namespace compute {

class context_intel : public context {
  using context::context;

public:
  context_intel() = default;
  explicit context_intel(const context &other) : context(other) {}

  device get_device() const {
    std::vector<device> devices = get_devices();

    if (devices.empty()) {
      return device();
    }

    return devices.front();
  }

  std::vector<device> get_devices() const {
    std::vector<cl_device_id> device_ids =
        get_info<std::vector<cl_device_id>>(CL_CONTEXT_DEVICES);

    std::vector<device> devices;
    for (size_t i = 0; i < device_ids.size(); i++) {
      devices.push_back(device(device_ids[i]));
    }
    return devices;
  }
};

} // namespace compute
} // namespace boost

#endif // BOOST_COMPUTE_INTEL_CONTEXT_HPP
