/*
 * Copyright (C) 2020-2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_ZE_INFO_JSON_FORMATTER_HPP
#define COMPUTE_SAMPLES_ZE_INFO_JSON_FORMATTER_HPP

#include "ze_info/capabilities.hpp"
#include "ze_api.h"
#include "zet_api.h"

#include <boost/property_tree/ptree.hpp>

namespace compute_samples {
boost::property_tree::ptree drivers_capabilities_to_json(
    const std::vector<DriverCapabilities> &capabilities);

boost::property_tree::ptree
driver_capabilities_to_json(const DriverCapabilities &capabilities);

boost::property_tree::ptree
driver_api_version_to_json(const ze_api_version_t &v);

boost::property_tree::ptree
driver_properties_to_json(const ze_driver_properties_t &p);

boost::property_tree::ptree
driver_ipc_properties_to_json(const ze_driver_ipc_properties_t &p);

boost::property_tree::ptree all_driver_extension_properties_to_json(
    const std::vector<ze_driver_extension_properties_t> &p);

boost::property_tree::ptree
driver_extension_properties_to_json(const ze_driver_extension_properties_t &p);

boost::property_tree::ptree
device_capabilities_to_json(const DeviceCapabilities &capabilities);

boost::property_tree::ptree
device_properties_to_json(const ze_device_properties_t &p);

boost::property_tree::ptree
device_compute_properties_to_json(const ze_device_compute_properties_t &p);

boost::property_tree::ptree
device_module_properties_to_json(const ze_device_module_properties_t &p);

boost::property_tree::ptree all_device_command_queue_group_properties_to_json(
    const std::vector<ze_command_queue_group_properties_t> &p);

boost::property_tree::ptree device_command_queue_group_properties_to_json(
    const ze_command_queue_group_properties_t &p);

boost::property_tree::ptree all_device_memory_properties_to_json(
    const std::vector<ze_device_memory_properties_t> &p);

boost::property_tree::ptree
device_memory_properties_to_json(const ze_device_memory_properties_t &p);

boost::property_tree::ptree device_memory_access_properties_to_json(
    const ze_device_memory_access_properties_t &p);

boost::property_tree::ptree all_device_cache_properties_to_json(
    const std::vector<ze_device_cache_properties_t> &p);

boost::property_tree::ptree
device_cache_properties_to_json(const ze_device_cache_properties_t &p);

boost::property_tree::ptree
device_image_properties_to_json(const ze_device_image_properties_t &p);

boost::property_tree::ptree device_external_memory_properties_to_json(
    const ze_device_external_memory_properties_t &p);

boost::property_tree::ptree
device_debug_properties_to_json(const zet_device_debug_properties_t &p);

boost::property_tree::ptree device_mutable_command_list_properties_to_json(
    const ze_mutable_command_list_exp_properties_t &p);

boost::property_tree::ptree tracer_metrics_flags_count_to_json(
    const std::map<zet_metric_group_sampling_type_flag_t, uint32_t>
        &flags_count);

template <typename PROPERTIES>
boost::property_tree::ptree
all_device_sysman_properties_to_json(const std::vector<PROPERTIES> &p);

boost::property_tree::ptree
device_sysman_properties_to_json(const zes_engine_properties_t &p);

boost::property_tree::ptree
device_sysman_properties_to_json(const zes_power_properties_t &p);

boost::property_tree::ptree
device_sysman_properties_to_json(const zes_diag_properties_t &p);

boost::property_tree::ptree
device_sysman_properties_to_json(const zes_mem_properties_t &p);

boost::property_tree::ptree
device_sysman_properties_to_json(const zes_freq_properties_t &p);

boost::property_tree::ptree
device_sysman_properties_to_json(const zes_temp_properties_t &p);

std::string ptree_to_string(const boost::property_tree::ptree tree);

void write_capabilities_to_json(
    const std::vector<DriverCapabilities> &capabilities,
    const std::string output);
} // namespace compute_samples

#endif
