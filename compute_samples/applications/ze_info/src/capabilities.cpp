/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ze_info/capabilities.hpp"

#include <vector>
#include <stdexcept>

#include "logging/logging.hpp"
#include "ze_utils/ze_utils.hpp"

namespace compute_samples {
std::vector<ze_driver_handle_t> get_drivers() {
  uint32_t count = 0;
  ze_result_t result = zeDriverGet(&count, nullptr);
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDriverGet failed: " + to_string(result));
  }
  LOG_DEBUG << "Driver count retrieved";

  std::vector<ze_driver_handle_t> drivers(count);
  result = zeDriverGet(&count, drivers.data());
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDriverGet failed: " + to_string(result));
  }
  LOG_DEBUG << "Driver handles retrieved";
  return drivers;
}

std::vector<DriverCapabilities>
get_drivers_capabilities(const std::vector<ze_driver_handle_t> drivers) {
  std::vector<DriverCapabilities> driver_capabilities(drivers.size());
  for (size_t i = 0; i < driver_capabilities.size(); ++i) {
    driver_capabilities[i] = get_driver_capabilities(drivers[i]);
  }
  return driver_capabilities;
}

DriverCapabilities get_driver_capabilities(const ze_driver_handle_t driver) {
  DriverCapabilities capabilities;
  capabilities.api_version = get_driver_api_version(driver);
  capabilities.driver_properties = get_driver_properties(driver);
  capabilities.ipc_properties = get_driver_ipc_properties(driver);
  capabilities.devices =
      get_driver_devices_capabilities(get_driver_devices(driver));
  return capabilities;
}

ze_driver_properties_t get_driver_properties(const ze_driver_handle_t driver) {
  ze_driver_properties_t properties = {ZE_DRIVER_PROPERTIES_VERSION_CURRENT};
  ze_result_t result = zeDriverGetProperties(driver, &properties);
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDriverGetProperties failed: " +
                             to_string(result));
  }
  LOG_DEBUG << "Driver properties retrieved";
  return properties;
}

ze_api_version_t get_driver_api_version(const ze_driver_handle_t driver) {
  ze_api_version_t version;
  ze_result_t result = zeDriverGetApiVersion(driver, &version);
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDriverGetApiVersion failed: " +
                             to_string(result));
  }
  LOG_DEBUG << "Driver API version retrieved";
  return version;
}

ze_driver_ipc_properties_t
get_driver_ipc_properties(const ze_driver_handle_t driver) {
  ze_driver_ipc_properties_t properties = {
      ZE_DRIVER_IPC_PROPERTIES_VERSION_CURRENT};
  ze_result_t result = zeDriverGetIPCProperties(driver, &properties);
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDriverGetIPCProperties failed: " +
                             to_string(result));
  }
  LOG_DEBUG << "Driver IPC properties retrieved";
  return properties;
}

std::vector<ze_device_handle_t>
get_driver_devices(const ze_driver_handle_t driver) {
  uint32_t count = 0;
  ze_result_t result = zeDeviceGet(driver, &count, nullptr);
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDeviceGet failed: " + to_string(result));
  }
  LOG_DEBUG << "Device count retrieved";

  std::vector<ze_device_handle_t> devices(count);
  result = zeDeviceGet(driver, &count, devices.data());
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDeviceGet failed: " + to_string(result));
  }
  LOG_DEBUG << "Device handles retrieved";
  return devices;
}

std::vector<DeviceCapabilities> get_driver_devices_capabilities(
    const std::vector<ze_device_handle_t> &devices) {
  std::vector<DeviceCapabilities> device_capabilities(devices.size());
  for (size_t i = 0; i < device_capabilities.size(); ++i) {
    device_capabilities[i] = get_device_capabilities(devices[i]);
  }
  return device_capabilities;
}

DeviceCapabilities get_device_capabilities(const ze_device_handle_t device) {
  DeviceCapabilities capabilities;
  capabilities.device_properties = get_device_properties(device);
  capabilities.compute_properties = get_device_compute_properties(device);
  capabilities.kernel_properties = get_device_kernel_properties(device);
  capabilities.memory_properties = get_device_memory_properties(device);
  capabilities.memory_access_properties =
      get_device_memory_access_properties(device);
  capabilities.cache_properties = get_device_cache_properties(device);
  capabilities.image_properties = get_device_image_properties(device);
  capabilities.sub_devices =
      get_device_sub_devices_capabilities(get_device_sub_devices(device));
  return capabilities;
}

ze_device_properties_t get_device_properties(const ze_device_handle_t device) {
  ze_device_properties_t properties = {ZE_DEVICE_PROPERTIES_VERSION_CURRENT};
  ze_result_t result = zeDeviceGetProperties(device, &properties);
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDeviceGetProperties failed: " +
                             to_string(result));
  }
  LOG_DEBUG << "Device properties retrieved";
  return properties;
}

ze_device_compute_properties_t
get_device_compute_properties(const ze_device_handle_t device) {
  ze_device_compute_properties_t properties = {
      ZE_DEVICE_COMPUTE_PROPERTIES_VERSION_CURRENT};
  ze_result_t result = zeDeviceGetComputeProperties(device, &properties);
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDeviceGetComputeProperties failed: " +
                             to_string(result));
  }
  LOG_DEBUG << "Device compute properties retrieved";
  return properties;
}

ze_device_kernel_properties_t
get_device_kernel_properties(const ze_device_handle_t device) {
  ze_device_kernel_properties_t properties = {
      ZE_DEVICE_KERNEL_PROPERTIES_VERSION_CURRENT};
  ze_result_t result = zeDeviceGetKernelProperties(device, &properties);
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDeviceGetKernelProperties failed: " +
                             to_string(result));
  }
  LOG_DEBUG << "Device kernel properties retrieved";
  return properties;
}

std::vector<ze_device_memory_properties_t>
get_device_memory_properties(const ze_device_handle_t device) {
  uint32_t count = 0;
  ze_result_t result = zeDeviceGetMemoryProperties(device, &count, nullptr);
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDeviceGetMemoryProperties failed: " +
                             to_string(result));
  }
  LOG_DEBUG << "Device memory properties count retrieved";

  std::vector<ze_device_memory_properties_t> properties(
      count, {ZE_DEVICE_MEMORY_PROPERTIES_VERSION_CURRENT});
  result = zeDeviceGetMemoryProperties(device, &count, properties.data());
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDeviceGetMemoryProperties failed: " +
                             to_string(result));
  }
  LOG_DEBUG << "Device memory properties retrieved";
  return properties;
}

ze_device_memory_access_properties_t
get_device_memory_access_properties(const ze_device_handle_t device) {
  ze_device_memory_access_properties_t properties = {
      ZE_DEVICE_MEMORY_ACCESS_PROPERTIES_VERSION_CURRENT};
  ze_result_t result = zeDeviceGetMemoryAccessProperties(device, &properties);
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDeviceGetMemoryAccessProperties failed: " +
                             to_string(result));
  }
  LOG_DEBUG << "Device memory access properties retrieved";
  return properties;
}

ze_device_cache_properties_t
get_device_cache_properties(const ze_device_handle_t device) {
  ze_device_cache_properties_t properties = {
      ZE_DEVICE_CACHE_PROPERTIES_VERSION_CURRENT};
  ze_result_t result = zeDeviceGetCacheProperties(device, &properties);
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDeviceGetCacheProperties failed: " +
                             to_string(result));
  }
  LOG_DEBUG << "Device cache properties retrieved";
  return properties;
}

ze_device_image_properties_t
get_device_image_properties(const ze_device_handle_t device) {
  ze_device_image_properties_t properties = {
      ZE_DEVICE_IMAGE_PROPERTIES_VERSION_CURRENT};
  ze_result_t result = zeDeviceGetImageProperties(device, &properties);
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDeviceGetImageProperties failed: " +
                             to_string(result));
  }
  LOG_DEBUG << "Device image properties retrieved";
  return properties;
}

std::vector<ze_device_handle_t>
get_device_sub_devices(const ze_device_handle_t device) {
  uint32_t count = 0;
  ze_result_t result = zeDeviceGetSubDevices(device, &count, nullptr);
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDeviceGetSubDevices failed: " +
                             to_string(result));
  }
  LOG_DEBUG << "Device sub-devices count retrieved";

  std::vector<ze_device_handle_t> sub_devices(count);
  result = zeDeviceGetSubDevices(device, &count, sub_devices.data());
  if (result != ZE_RESULT_SUCCESS) {
    throw std::runtime_error("zeDeviceGetSubDevices failed: " +
                             to_string(result));
  }
  LOG_DEBUG << "Device sub-devices retrieved";
  return sub_devices;
}

std::vector<DeviceCapabilities> get_device_sub_devices_capabilities(
    const std::vector<ze_device_handle_t> &sub_devices) {
  std::vector<DeviceCapabilities> sub_device_capabilities(sub_devices.size());
  for (size_t i = 0; i < sub_device_capabilities.size(); ++i) {
    sub_device_capabilities[i] = get_device_capabilities(sub_devices[i]);
  }
  return sub_device_capabilities;
}

} // namespace compute_samples
