/*
 * Copyright (C) 2020-2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_ZE_INFO_CAPABILITIES_HPP
#define COMPUTE_SAMPLES_ZE_INFO_CAPABILITIES_HPP

#include <vector>
#include <string>
#include <map>
#include "ze_api.h"
#include "zet_api.h"
#include "zes_api.h"

namespace compute_samples {
struct DeviceCapabilities {
  ze_device_properties_t device_properties;
  ze_device_compute_properties_t compute_properties;
  ze_device_module_properties_t module_properties;
  std::vector<ze_command_queue_group_properties_t>
      command_queue_group_properties;
  std::vector<ze_device_memory_properties_t> memory_properties;
  ze_device_memory_access_properties_t memory_access_properties;
  std::vector<ze_device_cache_properties_t> cache_properties;
  ze_device_image_properties_t image_properties;
  ze_device_external_memory_properties_t external_memory_properties;
  zet_device_debug_properties_t debug_properties;
  std::vector<DeviceCapabilities> sub_devices;
  ze_scheduling_hint_exp_properties_t scheduling_hint_properties;
  ze_float_atomic_ext_properties_t float_atomics_properties;
  ze_device_raytracing_ext_properties_t ray_tracing_properties;
  ze_mutable_command_list_exp_properties_t mutable_command_list_properties;
  std::map<zet_metric_group_sampling_type_flag_t, uint32_t>
      tracer_metrics_flags_count;
  std::vector<zes_engine_properties_t> sysman_engine_properties;
  std::vector<zes_diag_properties_t> sysman_diagnostic_properties;
  std::vector<zes_mem_properties_t> sysman_memory_properties;
  std::vector<zes_power_properties_t> sysman_power_properties;
  std::vector<zes_freq_properties_t> sysman_frequency_properties;
  std::vector<zes_temp_properties_t> sysman_temperature_properties;
  uint32_t programmable_metrics_count;
  uint32_t sysman_ras_handles_count;
  uint32_t sysman_vf_handles_count;
  uint32_t sysman_performance_handles_count;
  uint32_t sysman_firmware_handles_count;
  uint32_t sysman_standby_handles_count;
  uint32_t sysman_scheduler_handles_count;
  uint32_t sysman_pci_bars_count;
  ze_bool_t sysman_ecc_available;
  ze_bool_t sysman_ecc_configurable;
};

struct DriverCapabilities {
  ze_api_version_t api_version;
  ze_driver_properties_t driver_properties;
  ze_driver_ipc_properties_t ipc_properties;
  std::vector<ze_driver_extension_properties_t> extension_properties;
  std::vector<DeviceCapabilities> devices;
};

std::vector<ze_driver_handle_t> get_drivers();

std::vector<DriverCapabilities>
get_drivers_capabilities(const std::vector<ze_driver_handle_t> &drivers);

DriverCapabilities get_driver_capabilities(ze_driver_handle_t driver);

ze_api_version_t get_driver_api_version(ze_driver_handle_t driver);

ze_driver_properties_t get_driver_properties(ze_driver_handle_t driver);

ze_driver_ipc_properties_t get_driver_ipc_properties(ze_driver_handle_t driver);

std::vector<ze_driver_extension_properties_t>
get_driver_extension_properties(ze_driver_handle_t driver);

std::vector<ze_device_handle_t> get_driver_devices(ze_driver_handle_t driver);

std::vector<DeviceCapabilities>
get_driver_devices_capabilities(const std::vector<ze_device_handle_t> &devices);

DeviceCapabilities get_device_capabilities(ze_device_handle_t device);

ze_device_properties_t get_device_properties(ze_device_handle_t device);

ze_device_compute_properties_t
get_device_compute_properties(ze_device_handle_t device);

ze_device_module_properties_t
get_device_module_properties(ze_device_handle_t device);

std::vector<ze_command_queue_group_properties_t>
get_device_command_queue_group_properties(ze_device_handle_t device);

std::vector<ze_device_memory_properties_t>
get_device_memory_properties(ze_device_handle_t device);

ze_device_memory_access_properties_t
get_device_memory_access_properties(ze_device_handle_t device);

std::vector<ze_device_cache_properties_t>
get_device_cache_properties(ze_device_handle_t device);

ze_device_image_properties_t
get_device_image_properties(ze_device_handle_t device);

ze_device_external_memory_properties_t
get_device_external_memory_properties(ze_device_handle_t device);

zet_device_debug_properties_t
get_device_debug_properties(ze_device_handle_t device);

std::vector<ze_device_handle_t>
get_device_sub_devices(ze_device_handle_t device);

ze_scheduling_hint_exp_properties_t
get_device_kernel_schedule_hint_properties(ze_device_handle_t device);

ze_float_atomic_ext_properties_t
get_float_atomic_ext_properties(ze_device_handle_t device);

ze_device_raytracing_ext_properties_t
get_raytracing_ext_properties(ze_device_handle_t device);

ze_mutable_command_list_exp_properties_t
get_mutable_command_list_exp_properties(ze_device_handle_t device);

std::map<zet_metric_group_sampling_type_flag_t, uint32_t>
get_tracer_metrics_flags_count(ze_device_handle_t device);

uint32_t get_programmable_metrics_count(ze_device_handle_t device);

template <typename HANDLES, typename FN_HANDLES>
std::vector<HANDLES> get_sysman_handles(ze_device_handle_t device,
                                        FN_HANDLES fn_handles,
                                        const std::string &type);

template <typename PROPERTIES, typename HANDLES, typename FN_PROPERTIES>
std::vector<PROPERTIES>
get_sysman_properties(const std::vector<HANDLES> &handles,
                      FN_PROPERTIES fn_properties, const std::string &type);

template <typename FN_PROPERTY>
ze_bool_t get_sysman_bool_property(ze_device_handle_t device,
                                   FN_PROPERTY fn_property,
                                   const std::string &type);

std::vector<zes_engine_properties_t>
get_device_sysman_engine_properties(ze_device_handle_t device);

std::vector<zes_diag_properties_t>
get_device_sysman_diagnostic_properties(ze_device_handle_t device);

std::vector<zes_mem_properties_t>
get_device_sysman_memory_properties(ze_device_handle_t device);

std::vector<zes_power_properties_t>
get_device_sysman_power_properties(ze_device_handle_t device);

std::vector<zes_freq_properties_t>
get_device_sysman_frequency_properties(ze_device_handle_t device);

std::vector<zes_perf_properties_t>
get_device_sysman_performance_properties(ze_device_handle_t device);

std::vector<zes_temp_properties_t>
get_device_sysman_temperature_properties(ze_device_handle_t device);

uint32_t get_device_sysman_ras_handles_count(ze_device_handle_t device);

uint32_t get_device_sysman_vf_handles_count(ze_device_handle_t device);

uint32_t get_device_sysman_performance_handles_count(ze_device_handle_t device);

uint32_t get_device_sysman_firmware_handles_count(ze_device_handle_t device);

uint32_t get_device_sysman_standby_handles_count(ze_device_handle_t device);

uint32_t get_device_sysman_scheduler_handles_count(ze_device_handle_t device);

ze_bool_t get_device_sysman_ecc_available(ze_device_handle_t device);

ze_bool_t get_device_sysman_ecc_configurable(ze_device_handle_t device);

uint32_t get_device_pci_bars_count(ze_device_handle_t device);

std::vector<DeviceCapabilities> get_device_sub_devices_capabilities(
    const std::vector<ze_device_handle_t> &sub_devices);
} // namespace compute_samples

#endif
