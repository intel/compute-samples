/*
 * Copyright (C) 2020-2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ze_info/capabilities.hpp"
#include "ze_api.h"
#include "zet_api.h"
#include "zes_api.h"

#include <vector>

#include "logging/logging.hpp"
#include "ze_utils/ze_utils.hpp"

namespace compute_samples {

std::vector<ze_driver_handle_t> get_drivers() {
  auto result = ZE_RESULT_SUCCESS;

  ze_init_driver_type_desc_t desc = {ZE_STRUCTURE_TYPE_INIT_DRIVER_TYPE_DESC};
  desc.pNext = nullptr;
  desc.flags = UINT32_MAX; // all driver types requested

// prevent usage of ZES_ENABLE_SYSMMAN=1 (use zesInit instead)
#if defined(_WIN32) || defined(_WIN64)
  _putenv_s("ZES_ENABLE_SYSMAN", "0");
#else  // defined(_WIN32) || defined(_WIN64)
  setenv("ZES_ENABLE_SYSMAN", "0", 1);
#endif // defined(_WIN32) || defined(_WIN64)

  uint32_t count = 0;
  result = zeInitDrivers(&count, nullptr, &desc);
  throw_if_failed(result, "zeInitDrivers");
  LOG_DEBUG << "Driver count retrieved";

  std::vector<ze_driver_handle_t> drivers(count);
  result = zeInitDrivers(&count, drivers.data(), &desc);
  throw_if_failed(result, "zeInitDrivers");
  LOG_DEBUG << "Driver handles retrieved";

  result = zesInit(0);
  throw_if_failed(result, "zesInit");
  LOG_DEBUG << "Sysman drivers initialized";

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
  capabilities.scheduling_hint_properties =
      get_device_kernel_schedule_hint_properties(device);
  capabilities.float_atomics_properties =
      get_float_atomic_ext_properties(device);
  capabilities.ray_tracing_properties = get_raytracing_ext_properties(device);
  capabilities.debug_properties = get_device_debug_properties(device);
  capabilities.mutable_command_list_properties =
      get_mutable_command_list_exp_properties(device);
  capabilities.programmable_metrics_properties =
      get_programmable_metrics_properties(device);
  capabilities.tracer_metrics_properties =
      get_tracer_metrics_properties(device);
  capabilities.programmable_metrics_count =
      get_programmable_metrics_count(device);
  capabilities.sysman_engine_properties =
      get_device_sysman_engine_properties(device);
  capabilities.sysman_diagnostic_properties =
      get_device_sysman_diagnostic_properties(device);
  capabilities.sysman_memory_properties =
      get_device_sysman_memory_properties(device);
  capabilities.sysman_power_properties =
      get_device_sysman_power_properties(device);
  capabilities.sysman_frequency_properties =
      get_device_sysman_frequency_properties(device);
  capabilities.sysman_temperature_properties =
      get_device_sysman_temperature_properties(device);
  capabilities.sysman_ras_handles_count =
      get_device_sysman_ras_handles_count(device);
  capabilities.sysman_vf_handles_count =
      get_device_sysman_vf_handles_count(device);
  capabilities.sysman_performance_handles_count =
      get_device_sysman_performance_handles_count(device);
  capabilities.sysman_firmware_handles_count =
      get_device_sysman_firmware_handles_count(device);
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

ze_scheduling_hint_exp_properties_t
get_device_kernel_schedule_hint_properties(ze_device_handle_t device) {
  ze_device_module_properties_t properties;
  properties = {ZE_STRUCTURE_TYPE_DEVICE_MODULE_PROPERTIES};

  ze_scheduling_hint_exp_properties_t hints = {};
  properties.pNext = &hints;
  hints.stype = ZE_STRUCTURE_TYPE_SCHEDULING_HINT_EXP_PROPERTIES;
  const auto result = zeDeviceGetModuleProperties(device, &properties);
  throw_if_failed(result, "zeDeviceGetModuleProperties");
  LOG_DEBUG << "Device module properties scheduling hints retrieved";
  return hints;
}

ze_float_atomic_ext_properties_t
get_float_atomic_ext_properties(ze_device_handle_t device) {
  ze_device_module_properties_t properties;
  properties = {ZE_STRUCTURE_TYPE_DEVICE_MODULE_PROPERTIES};

  ze_float_atomic_ext_properties_t float_atomic = {};
  properties.pNext = &float_atomic;
  float_atomic.stype = ZE_STRUCTURE_TYPE_FLOAT_ATOMIC_EXT_PROPERTIES;
  const auto result = zeDeviceGetModuleProperties(device, &properties);
  throw_if_failed(result, "zeDeviceGetModuleProperties");
  LOG_DEBUG << "Device module properties float atomics retrieved";
  return float_atomic;
}

ze_device_raytracing_ext_properties_t
get_raytracing_ext_properties(ze_device_handle_t device) {
  ze_device_module_properties_t properties;
  properties = {ZE_STRUCTURE_TYPE_DEVICE_MODULE_PROPERTIES};

  ze_device_raytracing_ext_properties_t device_raytracing = {};
  properties.pNext = &device_raytracing;
  device_raytracing.stype = ZE_STRUCTURE_TYPE_DEVICE_RAYTRACING_EXT_PROPERTIES;
  const auto result = zeDeviceGetModuleProperties(device, &properties);
  throw_if_failed(result, "zeDeviceGetModuleProperties");
  LOG_DEBUG
      << "Device module properties device raytracing ext propertie retrieved";
  return device_raytracing;
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

zet_device_debug_properties_t
get_device_debug_properties(ze_device_handle_t device) {
  zet_device_debug_properties_t properties = {
      ZET_STRUCTURE_TYPE_DEVICE_DEBUG_PROPERTIES};
  const auto result = zetDeviceGetDebugProperties(device, &properties);
  throw_if_failed(result, "zetDeviceGetDebugProperties");
  LOG_DEBUG << "Device debug properties retrieved";
  return properties;
}

ze_mutable_command_list_exp_properties_t
get_mutable_command_list_exp_properties(ze_device_handle_t device) {
  ze_mutable_command_list_exp_properties_t mutable_command_list_exp_properties =
      {ZE_STRUCTURE_TYPE_MUTABLE_COMMAND_LIST_EXP_PROPERTIES, nullptr, 0, 0};
  ze_device_properties_t properties = {ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES};
  properties.pNext = &mutable_command_list_exp_properties;
  const auto result = zeDeviceGetProperties(device, &properties);
  throw_if_failed(result, "zeDeviceGetProperties");
  LOG_DEBUG << "Device mutable command list exp properties retrieved";
  return mutable_command_list_exp_properties;
}

std::vector<zet_metric_programmable_exp_properties_t>
get_programmable_metrics_properties(ze_device_handle_t device) {
  uint32_t count = 0;
  auto result = zetMetricProgrammableGetExp(device, &count, nullptr);
  throw_if_failed(result, "zetMetricProgrammableGetExp");
  LOG_DEBUG << "Programmable metrics count retrieved";

  std::vector<zet_metric_programmable_exp_handle_t> metric_handles(count);
  result = zetMetricProgrammableGetExp(device, &count, metric_handles.data());
  throw_if_failed(result, "zetMetricProgrammableGetExp");
  LOG_DEBUG << "Programmable metrics handles retrieved";

  std::vector<zet_metric_programmable_exp_properties_t> properties(
      count, {ZET_STRUCTURE_TYPE_METRIC_PROGRAMMABLE_EXP_PROPERTIES});
  for (uint32_t i = 0; i < count; ++i) {
    result = zetMetricProgrammableGetPropertiesExp(metric_handles[i],
                                                   &properties[i]);
    throw_if_failed(result, "zetMetricProgrammableGetPropertiesExp");
  }

  LOG_DEBUG << "Programmable metrics properties retrieved";
  return properties;
}

std::vector<zet_metric_group_properties_t>
get_tracer_metrics_properties(ze_device_handle_t device) {
  uint32_t count = 0;
  auto result = zetMetricGroupGet(device, &count, nullptr);
  throw_if_failed(result, "zetMetricGroupGet");
  LOG_DEBUG << "Tracer metrics count retrieved";

  std::vector<zet_metric_group_handle_t> metric_handles(count);
  result = zetMetricGroupGet(device, &count, metric_handles.data());
  throw_if_failed(result, "zetMetricGroupGet");
  LOG_DEBUG << "Tracer metrics handles retrieved";

  std::vector<zet_metric_group_properties_t> properties(
      count, {ZET_STRUCTURE_TYPE_METRIC_GROUP_PROPERTIES});
  for (uint32_t i = 0; i < count; ++i) {
    zetMetricGroupGetProperties(metric_handles[i], &properties[i]);
    throw_if_failed(result, "zetMetricGroupGetProperties");
  }

  LOG_DEBUG << "Tracer metrics properties retrieved";
  return properties;
}

uint32_t get_programmable_metrics_count(ze_device_handle_t device) {
  uint32_t count = 0;
  auto result = zetMetricProgrammableGetExp(device, &count, nullptr);
  throw_if_failed(result, "zetMetricProgrammableGetExp");
  return count;
}

template <typename HANDLES, typename FN_HANDLES>
std::vector<HANDLES> get_sysman_handles(ze_device_handle_t device,
                                        FN_HANDLES fn_handles,
                                        const std::string &type) {
  zes_device_handle_t sysman_device =
      get_sysman_device_from_core_device(device);
  uint32_t count = 0;
  auto result = fn_handles(sysman_device, &count, nullptr);
  if (result != ZE_RESULT_SUCCESS) {
    LOG_WARNING << type + " failed: " + to_string(result) +
                       " (get_sysman_handles template)";
    return std::vector<HANDLES>(0);
  }
  LOG_DEBUG << "Device " + type +
                   " handle count retrieved (get_sysman_handles template)";

  std::vector<HANDLES> handles(count);
  result = fn_handles(sysman_device, &count, handles.data());
  if (result != ZE_RESULT_SUCCESS) {
    LOG_WARNING << type + " failed: " + to_string(result) +
                       " (get_sysman_handles template)";
    return std::vector<HANDLES>(0);
  }
  LOG_DEBUG << "Device " + type +
                   " handles retrieved (get_sysman_handles template)";
  return handles;
}

template <typename PROPERTIES, typename HANDLES, typename FN_PROPERTIES>
std::vector<PROPERTIES>
get_sysman_properties(const std::vector<HANDLES> &handles,
                      FN_PROPERTIES fn_properties, const std::string &type) {
  std::vector<PROPERTIES> properties(handles.size());
  if (handles.empty()) {
    return properties;
  }
  for (size_t i = 0; i < properties.size(); ++i) {
    auto result = fn_properties(handles[i], &properties[i]);
    if (result != ZE_RESULT_SUCCESS) {
      LOG_WARNING << type + " failed: " + to_string(result) +
                         " (get_sysman_handles template)";
      return std::vector<PROPERTIES>(0);
    }
  }
  LOG_DEBUG << "Device " + type +
                   " properties retrieved (get_sysman_properties template)";
  return properties;
}

std::vector<zes_engine_properties_t>
get_device_sysman_engine_properties(ze_device_handle_t device) {
  auto handles = get_sysman_handles<zes_engine_handle_t>(
      device, zesDeviceEnumEngineGroups, "zesDeviceEnumEngineGroups");

  return get_sysman_properties<zes_engine_properties_t, zes_engine_handle_t>(
      handles, zesEngineGetProperties, "zesEngineGetProperties");
}

std::vector<zes_diag_properties_t>
get_device_sysman_diagnostic_properties(ze_device_handle_t device) {
  auto handles = get_sysman_handles<zes_diag_handle_t>(
      device, zesDeviceEnumDiagnosticTestSuites,
      "zesDeviceEnumDiagnosticTestSuites");

  return get_sysman_properties<zes_diag_properties_t, zes_diag_handle_t>(
      handles, zesDiagnosticsGetProperties, "zesDiagnosticsGetProperties");
}

std::vector<zes_mem_properties_t>
get_device_sysman_memory_properties(ze_device_handle_t device) {
  auto handles = get_sysman_handles<zes_mem_handle_t>(
      device, zesDeviceEnumMemoryModules, "zesDeviceEnumMemoryModules");

  return get_sysman_properties<zes_mem_properties_t, zes_mem_handle_t>(
      handles, zesMemoryGetProperties, "zesMemoryGetProperties");
}

std::vector<zes_power_properties_t>
get_device_sysman_power_properties(ze_device_handle_t device) {
  auto handles = get_sysman_handles<zes_pwr_handle_t>(
      device, zesDeviceEnumPowerDomains, "zesDeviceEnumPowerDomains");

  return get_sysman_properties<zes_power_properties_t, zes_pwr_handle_t>(
      handles, zesPowerGetProperties, "zesPowerGetProperties");
}

std::vector<zes_firmware_properties_t>
get_device_sysman_firmware_properties(ze_device_handle_t device) {
  auto handles = get_sysman_handles<zes_firmware_handle_t>(
      device, zesDeviceEnumFirmwares, "zesDeviceEnumFirmwares");

  return get_sysman_properties<zes_firmware_properties_t,
                               zes_firmware_handle_t>(
      handles, zesFirmwareGetProperties, "zesFirmwareGetProperties");
}

std::vector<zes_freq_properties_t>
get_device_sysman_frequency_properties(ze_device_handle_t device) {
  auto handles = get_sysman_handles<zes_freq_handle_t>(
      device, zesDeviceEnumFrequencyDomains, "zesDeviceEnumFrequencyDomains");

  return get_sysman_properties<zes_freq_properties_t, zes_freq_handle_t>(
      handles, zesFrequencyGetProperties, "zesFrequencyGetProperties");
}

std::vector<zes_temp_properties_t>
get_device_sysman_temperature_properties(ze_device_handle_t device) {
  auto handles = get_sysman_handles<zes_temp_handle_t>(
      device, zesDeviceEnumTemperatureSensors,
      "zesDeviceEnumTemperatureSensors");

  return get_sysman_properties<zes_temp_properties_t, zes_temp_handle_t>(
      handles, zesTemperatureGetProperties, "zesTemperatureGetProperties");
}

uint32_t get_device_sysman_ras_handles_count(ze_device_handle_t device) {
  const auto handles = get_sysman_handles<zes_ras_handle_t>(
      device, zesDeviceEnumRasErrorSets, "zesDeviceEnumRasErrorSets");

  return handles.size();
}

uint32_t get_device_sysman_vf_handles_count(ze_device_handle_t device) {
  const auto handles = get_sysman_handles<zes_vf_handle_t>(
      device, zesDeviceEnumEnabledVFExp, "zesDeviceEnumEnabledVFExp");

  return handles.size();
}

uint32_t
get_device_sysman_performance_handles_count(ze_device_handle_t device) {
  auto handles = get_sysman_handles<zes_perf_handle_t>(
      device, zesDeviceEnumPerformanceFactorDomains,
      "zesDeviceEnumPerformanceFactorDomains");

  return handles.size();
}

uint32_t get_device_sysman_firmware_handles_count(ze_device_handle_t device) {
  auto handles = get_sysman_handles<zes_firmware_handle_t>(
      device, zesDeviceEnumFirmwares, "zesDeviceEnumFirmwares");

  return handles.size();
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
