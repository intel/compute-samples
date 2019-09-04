/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "vme_interop/vme_interop.hpp"

#include <boost/compute/utility.hpp>

#include "logging/logging.hpp"

namespace compute_samples {

void VmeInteropApplication::run_os_specific_implementation(
    std::vector<std::string> &, const Arguments &,
    const compute::device &) const {
  LOG_INFO << "VA API interoperability not supported on platform.";
}

} // namespace compute_samples
