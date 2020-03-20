/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_ZE_INFO_CAPABILITIES_HPP
#define COMPUTE_SAMPLES_ZE_INFO_CAPABILITIES_HPP

#include <vector>
#include "ze_api.h"

namespace compute_samples {
struct DeviceCapabilities {
  ze_device_properties_t device_properties;
  ze_device_compute_properties_t compute_properties;
  ze_device_kernel_properties_t kernel_properties;
  std::vector<ze_device_memory_properties_t> memory_properties;
  ze_device_memory_access_properties_t memory_access_properties;
  ze_device_cache_properties_t cache_properties;
  ze_device_image_properties_t image_properties;
  std::vector<DeviceCapabilities> sub_devices;
};

struct DriverCapabilities {
  ze_api_version_t api_version;
  ze_driver_properties_t driver_properties;
  ze_driver_ipc_properties_t ipc_properties;
  std::vector<DeviceCapabilities> devices;
};

std::vector<ze_driver_handle_t> get_drivers();

std::vector<DriverCapabilities>
get_drivers_capabilities(const std::vector<ze_driver_handle_t> drivers);

DriverCapabilities get_driver_capabilities(const ze_driver_handle_t driver);

ze_driver_properties_t get_driver_properties(const ze_driver_handle_t driver);

ze_api_version_t get_driver_api_version(const ze_driver_handle_t driver);

ze_driver_ipc_properties_t
get_driver_ipc_properties(const ze_driver_handle_t driver);

std::vector<ze_device_handle_t>
get_driver_devices(const ze_driver_handle_t driver);

std::vector<DeviceCapabilities>
get_driver_devices_capabilities(const std::vector<ze_device_handle_t> &devices);

DeviceCapabilities get_device_capabilities(const ze_device_handle_t device);

ze_device_properties_t get_device_properties(const ze_device_handle_t device);

ze_device_compute_properties_t
get_device_compute_properties(const ze_device_handle_t device);

ze_device_kernel_properties_t
get_device_kernel_properties(const ze_device_handle_t device);

std::vector<ze_device_memory_properties_t>
get_device_memory_properties(const ze_device_handle_t device);

ze_device_memory_access_properties_t
get_device_memory_access_properties(const ze_device_handle_t device);

ze_device_cache_properties_t
get_device_cache_properties(const ze_device_handle_t device);

ze_device_image_properties_t
get_device_image_properties(const ze_device_handle_t device);

std::vector<ze_device_handle_t>
get_device_sub_devices(const ze_device_handle_t device);

std::vector<DeviceCapabilities> get_device_sub_devices_capabilities(
    const std::vector<ze_device_handle_t> &sub_devices);

} // namespace compute_samples

#endif
