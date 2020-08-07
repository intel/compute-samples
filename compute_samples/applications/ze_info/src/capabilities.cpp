/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ze_info/capabilities.hpp"
#include "ze_api.h"

#include <vector>

#include "logging/logging.hpp"
#include "ze_utils/ze_utils.hpp"

namespace compute_samples {

std::vector<ze_driver_handle_t> get_drivers() {
  auto result = ZE_RESULT_SUCCESS;

  uint32_t count = 0;
  result = zeDriverGet(&count, nullptr);
  throw_if_failed(result, "zeDriverGet");
  LOG_DEBUG << "Driver count retrieved";

  std::vector<ze_driver_handle_t> drivers(count);
  result = zeDriverGet(&count, drivers.data());
  throw_if_failed(result, "zeDriverGet");
  LOG_DEBUG << "Driver handles retrieved";

  return drivers;
}

std::vector<DriverCapabilities>
get_drivers_capabilities(const std::vector<ze_driver_handle_t> &drivers) {
  std::vector<DriverCapabilities> driver_capabilities(drivers.size());
  for (size_t i = 0; i < driver_capabilities.size(); ++i) {
    driver_capabilities[i] = get_driver_capabilities(drivers[i]);
  }
  return driver_capabilities;
}

DriverCapabilities get_driver_capabilities(ze_driver_handle_t driver) {
  DriverCapabilities capabilities;
  capabilities.api_version = get_driver_api_version(driver);
  capabilities.driver_properties = get_driver_properties(driver);
  capabilities.ipc_properties = get_driver_ipc_properties(driver);
  capabilities.extension_properties = get_driver_extension_properties(driver);
  capabilities.devices =
      get_driver_devices_capabilities(get_driver_devices(driver));
  return capabilities;
}

ze_api_version_t get_driver_api_version(ze_driver_handle_t driver) {
  ze_api_version_t version = ZE_API_VERSION_CURRENT;
  const auto result = zeDriverGetApiVersion(driver, &version);
  throw_if_failed(result, "zeDriverGetApiVersion");
  LOG_DEBUG << "Driver API version retrieved";
  return version;
}

ze_driver_properties_t get_driver_properties(ze_driver_handle_t driver) {
  ze_driver_properties_t properties = {ZE_STRUCTURE_TYPE_DRIVER_PROPERTIES};
  const auto result = zeDriverGetProperties(driver, &properties);
  throw_if_failed(result, "zeDriverGetProperties");
  LOG_DEBUG << "Driver properties retrieved";
  return properties;
}

ze_driver_ipc_properties_t
get_driver_ipc_properties(ze_driver_handle_t driver) {
  ze_driver_ipc_properties_t properties = {
      ZE_STRUCTURE_TYPE_DRIVER_IPC_PROPERTIES};
  const auto result = zeDriverGetIpcProperties(driver, &properties);
  throw_if_failed(result, "zeDriverGetProperties");
  LOG_DEBUG << "Driver IPC properties retrieved";
  return properties;
}

std::vector<ze_driver_extension_properties_t>
get_driver_extension_properties(ze_driver_handle_t driver) {
  auto result = ZE_RESULT_SUCCESS;

  uint32_t count = 0;
  result = zeDriverGetExtensionProperties(driver, &count, nullptr);
  throw_if_failed(result, "zeDriverGetExtensionProperties");
  LOG_DEBUG << "Driver extension properties count retrieved";

  std::vector<ze_driver_extension_properties_t> properties(count);
  result = zeDriverGetExtensionProperties(driver, &count, properties.data());
  throw_if_failed(result, "zeDriverGetExtensionProperties");
  LOG_DEBUG << "Driver extension properties retrieved";

  return properties;
}

std::vector<ze_device_handle_t> get_driver_devices(ze_driver_handle_t driver) {
  auto result = ZE_RESULT_SUCCESS;

  uint32_t count = 0;
  result = zeDeviceGet(driver, &count, nullptr);
  throw_if_failed(result, "zeDeviceGet");
  LOG_DEBUG << "Device count retrieved";

  std::vector<ze_device_handle_t> devices(count);
  result = zeDeviceGet(driver, &count, devices.data());
  throw_if_failed(result, "zeDeviceGet");
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

DeviceCapabilities get_device_capabilities(ze_device_handle_t device) {
  DeviceCapabilities capabilities;
  capabilities.device_properties = get_device_properties(device);
  capabilities.compute_properties = get_device_compute_properties(device);
  capabilities.module_properties = get_device_module_properties(device);
  capabilities.command_queue_group_properties =
      get_device_command_queue_group_properties(device);
  capabilities.memory_properties = get_device_memory_properties(device);
  capabilities.memory_access_properties =
      get_device_memory_access_properties(device);
  capabilities.cache_properties = get_device_cache_properties(device);
  capabilities.image_properties = get_device_image_properties(device);
  capabilities.external_memory_properties =
      get_device_external_memory_properties(device);
  capabilities.sub_devices =
      get_device_sub_devices_capabilities(get_device_sub_devices(device));
  return capabilities;
}

ze_device_properties_t get_device_properties(ze_device_handle_t device) {
  ze_device_properties_t properties = {ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES};
  const auto result = zeDeviceGetProperties(device, &properties);
  throw_if_failed(result, "zeDeviceGetProperties");
  LOG_DEBUG << "Device properties retrieved";
  return properties;
}

ze_device_compute_properties_t
get_device_compute_properties(ze_device_handle_t device) {
  ze_device_compute_properties_t properties = {
      ZE_STRUCTURE_TYPE_DEVICE_COMPUTE_PROPERTIES};
  const auto result = zeDeviceGetComputeProperties(device, &properties);
  throw_if_failed(result, "zeDeviceGetComputeProperties");
  LOG_DEBUG << "Device compute properties retrieved";
  return properties;
}

ze_device_module_properties_t
get_device_module_properties(ze_device_handle_t device) {
  ze_device_module_properties_t properties = {
      ZE_STRUCTURE_TYPE_MODULE_PROPERTIES};
  const auto result = zeDeviceGetModuleProperties(device, &properties);
  throw_if_failed(result, "zeDeviceGetModuleProperties");
  LOG_DEBUG << "Device module properties retrieved";
  return properties;
}

std::vector<ze_command_queue_group_properties_t>
get_device_command_queue_group_properties(ze_device_handle_t device) {
  auto result = ZE_RESULT_SUCCESS;

  uint32_t count = 0;
  result = zeDeviceGetCommandQueueGroupProperties(device, &count, nullptr);
  throw_if_failed(result, "zeDeviceGetCommandQueueGroupProperties");
  LOG_DEBUG << "Device command queue group properties count retrieved";

  std::vector<ze_command_queue_group_properties_t> properties(
      count, {ZE_STRUCTURE_TYPE_COMMAND_QUEUE_GROUP_PROPERTIES});
  result =
      zeDeviceGetCommandQueueGroupProperties(device, &count, properties.data());
  throw_if_failed(result, "zeDeviceGetCommandQueueGroupProperties");
  LOG_DEBUG << "Device command queue group properties retrieved";

  return properties;
}

std::vector<ze_device_memory_properties_t>
get_device_memory_properties(ze_device_handle_t device) {
  auto result = ZE_RESULT_SUCCESS;

  uint32_t count = 0;
  result = zeDeviceGetMemoryProperties(device, &count, nullptr);
  throw_if_failed(result, "zeDeviceGetMemoryProperties");
  LOG_DEBUG << "Device memory properties count retrieved";

  std::vector<ze_device_memory_properties_t> properties(
      count, {ZE_STRUCTURE_TYPE_DEVICE_MEMORY_PROPERTIES});
  result = zeDeviceGetMemoryProperties(device, &count, properties.data());
  throw_if_failed(result, "zeDeviceGetMemoryProperties");
  LOG_DEBUG << "Device memory properties retrieved";

  return properties;
}

ze_device_memory_access_properties_t
get_device_memory_access_properties(ze_device_handle_t device) {
  ze_device_memory_access_properties_t properties = {
      ZE_STRUCTURE_TYPE_DEVICE_MEMORY_ACCESS_PROPERTIES};
  const auto result = zeDeviceGetMemoryAccessProperties(device, &properties);
  throw_if_failed(result, "zeDeviceGetMemoryProperties");
  LOG_DEBUG << "Device memory access properties retrieved";
  return properties;
}

std::vector<ze_device_cache_properties_t>
get_device_cache_properties(ze_device_handle_t device) {
  auto result = ZE_RESULT_SUCCESS;

  uint32_t count = 0;
  result = zeDeviceGetCacheProperties(device, &count, nullptr);
  throw_if_failed(result, "zeDeviceGetCacheProperties");
  LOG_DEBUG << "Device cache properties count retrieved";

  std::vector<ze_device_cache_properties_t> properties(
      count, {ZE_STRUCTURE_TYPE_DEVICE_CACHE_PROPERTIES});
  result = zeDeviceGetCacheProperties(device, &count, properties.data());
  throw_if_failed(result, "zeDeviceGetCacheProperties");
  LOG_DEBUG << "Device cache properties retrieved";

  return properties;
}

ze_device_image_properties_t
get_device_image_properties(ze_device_handle_t device) {
  ze_device_image_properties_t properties = {
      ZE_STRUCTURE_TYPE_DEVICE_IMAGE_PROPERTIES};
  const auto result = zeDeviceGetImageProperties(device, &properties);
  throw_if_failed(result, "zeDeviceGetImageProperties");
  LOG_DEBUG << "Device image properties retrieved";
  return properties;
}

ze_device_external_memory_properties_t
get_device_external_memory_properties(ze_device_handle_t device) {
  ze_device_external_memory_properties_t properties = {
      ZE_STRUCTURE_TYPE_DEVICE_EXTERNAL_MEMORY_PROPERTIES};
  const auto result = zeDeviceGetExternalMemoryProperties(device, &properties);
  throw_if_failed(result, "zeDeviceGetExternalMemoryProperties");
  LOG_DEBUG << "Device external memory properties retrieved";
  return properties;
}

std::vector<ze_device_handle_t>
get_device_sub_devices(ze_device_handle_t device) {
  auto result = ZE_RESULT_SUCCESS;

  uint32_t count = 0;
  result = zeDeviceGetSubDevices(device, &count, nullptr);
  throw_if_failed(result, "zeDeviceGetSubDevices");
  LOG_DEBUG << "Device sub-devices count retrieved";

  std::vector<ze_device_handle_t> sub_devices(count);
  result = zeDeviceGetSubDevices(device, &count, sub_devices.data());
  throw_if_failed(result, "zeDeviceGetSubDevices");
  LOG_DEBUG << "Device sub-devices properties retrieved";

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
