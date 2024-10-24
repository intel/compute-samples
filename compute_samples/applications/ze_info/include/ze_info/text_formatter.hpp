/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_ZE_INFO_TEXT_FORMATTER_HPP
#define COMPUTE_SAMPLES_ZE_INFO_TEXT_FORMATTER_HPP

#include "ze_info/capabilities.hpp"
#include "ze_api.h"
#include "zet_api.h"

#include <string>
#include <vector>

namespace compute_samples {
std::string drivers_capabilities_to_text(
    const std::vector<DriverCapabilities> &capabilities, int indentation_level);

std::string driver_capabilities_to_text(const DriverCapabilities &capabilities,
                                        int indentation_level);

std::string driver_api_version_to_text(const ze_api_version_t &v,
                                       int indentation_level);

std::string driver_properties_to_text(const ze_driver_properties_t &p,
                                      int indentation_level);

std::string driver_ipc_properties_to_text(const ze_driver_ipc_properties_t &p,
                                          int indentation_level);

std::string all_driver_extension_properties_to_text(
    const std::vector<ze_driver_extension_properties_t> &p,
    int indentation_level);

std::string
driver_extension_properties_to_text(const ze_driver_extension_properties_t &p,
                                    int indentation_level);

std::string device_capabilities_to_text(const DeviceCapabilities &capabilities,
                                        int indentation_level);

std::string device_properties_to_text(const ze_device_properties_t &p,
                                      int indentation_level);

std::string
device_compute_properties_to_text(const ze_device_compute_properties_t &p,
                                  int indentation_level);

std::string
device_module_properties_to_text(const ze_device_module_properties_t &p,
                                 int indentation_level);

std::string all_device_command_queue_group_properties_to_text(
    const std::vector<ze_command_queue_group_properties_t> &p,
    int indentation_level);

std::string device_command_queue_group_properties_to_text(
    const ze_command_queue_group_properties_t &p, int indentation_level);

std::string all_device_memory_properties_to_text(
    const std::vector<ze_device_memory_properties_t> &p, int indentation_level);

std::string
device_memory_properties_to_text(const ze_device_memory_properties_t &p,
                                 int indentation_level);

std::string device_memory_access_properties_to_text(
    const ze_device_memory_access_properties_t &p, int indentation_level);

std::string all_device_cache_properties_to_text(
    const std::vector<ze_device_cache_properties_t> &p, int indentation_level);

std::string
device_cache_properties_to_text(const ze_device_cache_properties_t &p,
                                int indentation_level);

std::string
device_image_properties_to_text(const ze_device_image_properties_t &p,
                                int indentation_level);

std::string device_external_memory_properties_to_text(
    const ze_device_external_memory_properties_t &p, int indentation_level);

std::string
device_debug_properties_to_text(const zet_device_debug_properties_t &p,
                                int indentation_level);

std::string key_value_to_text(const std::string key, const std::string value,
                              int indentation_level);

std::string kernel_scheduling_hint_properties_to_text(
    const ze_scheduling_hint_exp_properties_t &p, const int indentation_level);

std::string
float_atomics_properties_to_text(const ze_float_atomic_ext_properties_t &p,
                                 const int indentation_level);

std::string device_raytracing_properties_to_text(
    const ze_device_raytracing_ext_properties_t &p,
    const int indentation_level);

std::string device_mutable_command_list_properties_to_text(
    const ze_mutable_command_list_exp_properties_t &p,
    const int indentation_level);

} // namespace compute_samples

#endif
