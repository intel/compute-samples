/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_ZE_INFO_TEXT_FORMATTER_HPP
#define COMPUTE_SAMPLES_ZE_INFO_TEXT_FORMATTER_HPP

#include "ze_info/capabilities.hpp"

#include <string>
#include <vector>

namespace compute_samples {
std::string
drivers_capabilities_to_text(const std::vector<DriverCapabilities> capabilities,
                             const int indentation_level);

std::string driver_capabilities_to_text(const DriverCapabilities capabilities,
                                        const int indentation_level);

std::string driver_properties_to_text(const ze_driver_properties_t p,
                                      const int indentation_level);

std::string driver_api_version_to_text(const ze_api_version_t v,
                                       const int indentation_level);

std::string driver_ipc_properties_to_text(const ze_driver_ipc_properties_t p,
                                          const int indentation_level);

std::string device_capabilities_to_text(const DeviceCapabilities capabilities,
                                        const int indentation_level);

std::string device_properties_to_text(const ze_device_properties_t p,
                                      const int indentation_level);

std::string
device_compute_properties_to_text(const ze_device_compute_properties_t p,
                                  const int indentation_level);

std::string
device_kernel_properties_to_text(const ze_device_kernel_properties_t p,
                                 const int indentation_level);

std::string all_device_memory_properties_to_text(
    const std::vector<ze_device_memory_properties_t> &p,
    const int indentation_level);

std::string
device_memory_properties_to_text(const ze_device_memory_properties_t p,
                                 const int indentation_level);

std::string device_memory_access_properties_to_text(
    const ze_device_memory_access_properties_t p, const int indentation_level);

std::string
device_cache_properties_to_text(const ze_device_cache_properties_t p,
                                const int indentation_level);

std::string
device_image_properties_to_text(const ze_device_image_properties_t p,
                                const int indentation_level);

std::string key_value_to_text(const std::string key, const std::string value,
                              const int indentation_level);

} // namespace compute_samples

#endif
