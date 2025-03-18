/*
 * Copyright (C) 2020-2025 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ze_info/json_formatter.hpp"

#include "ze_api.h"
#include "zet_api.h"
#include "utils/utils.hpp"
#include "ze_utils/ze_utils.hpp"

#include <sstream>
#include <string>
#include <iomanip>
#include <regex>
#include <map>

#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;

namespace compute_samples {

boost::property_tree::ptree drivers_capabilities_to_json(
    const std::vector<DriverCapabilities> &capabilities) {
  pt::ptree tree;
  if (capabilities.empty()) {
    pt::ptree node;
    tree.push_back(std::make_pair("", node));
  } else {
    for (const auto &capability : capabilities) {
      tree.push_back(
          std::make_pair("", driver_capabilities_to_json(capability)));
    }
  }
  return tree;
}

boost::property_tree::ptree
driver_capabilities_to_json(const DriverCapabilities &capabilities) {
  pt::ptree tree;
  const auto int_version = static_cast<uint32_t>(capabilities.api_version);
  const std::string string_version =
      std::to_string(ZE_MAJOR_VERSION(int_version)) + "." +
      std::to_string(ZE_MINOR_VERSION(int_version));
  tree.put("ze_api_version_t", string_version);
  tree.add_child("ze_driver_properties_t",
                 driver_properties_to_json(capabilities.driver_properties));
  tree.add_child("ze_driver_ipc_properties_t",
                 driver_ipc_properties_to_json(capabilities.ipc_properties));
  tree.add_child("ze_driver_extension_properties_t",
                 all_driver_extension_properties_to_json(
                     capabilities.extension_properties));
  pt::ptree devices;
  if (capabilities.devices.empty()) {
    pt::ptree node;
    devices.push_back(std::make_pair("", node));
  } else {
    for (const auto &sub_device : capabilities.devices) {
      devices.push_back(
          std::make_pair("", device_capabilities_to_json(sub_device)));
    }
  }
  tree.add_child("devices", devices);
  return tree;
}

boost::property_tree::ptree
driver_properties_to_json(const ze_driver_properties_t &p) {
  pt::ptree tree;
  tree.put("uuid", uuid_to_string(p.uuid.id));
  tree.put("driverVersion", p.driverVersion);
  return tree;
}

boost::property_tree::ptree
driver_ipc_properties_to_json(const ze_driver_ipc_properties_t &p) {
  pt::ptree tree;
  pt::ptree flags_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_ipc_property_flag_t>(p.flags), " | ")) {
    pt::ptree node;
    node.put("", flag);
    flags_node.push_back(std::make_pair("", node));
  }
  tree.add_child("flags", flags_node);
  return tree;
}

boost::property_tree::ptree all_driver_extension_properties_to_json(
    const std::vector<ze_driver_extension_properties_t> &p) {
  pt::ptree tree;
  if (p.empty()) {
    pt::ptree node;
    tree.push_back(std::make_pair("", node));
  } else {
    std::map<std::string, std::string> extensions;
    for (const auto &properties : p) {
      const auto int_version = properties.version;
      const std::string string_version =
          std::to_string(ZE_MAJOR_VERSION(int_version)) + "." +
          std::to_string(ZE_MINOR_VERSION(int_version));
      extensions[properties.name] = string_version;
    }

    for (const auto &item : extensions) {
      pt::ptree extensions_tree;
      extensions_tree.put("name", item.first);
      extensions_tree.put("version", item.second);
      tree.push_back(std::make_pair("", extensions_tree));
    }
  }
  return tree;
}

boost::property_tree::ptree kernel_scheduling_hint_properties_to_json(
    const ze_scheduling_hint_exp_properties_t &p) {
  pt::ptree tree;
  pt::ptree flags_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_scheduling_hint_exp_flag_t>(
                        p.schedulingHintFlags),
                    " | ")) {
    pt::ptree node;
    node.put("", flag);
    flags_node.push_back(std::make_pair("", node));
  }
  tree.add_child("schedulingHintFlags", flags_node);
  return tree;
}

boost::property_tree::ptree
float_atomics_properties_to_json(const ze_float_atomic_ext_properties_t &p) {
  pt::ptree tree;
  pt::ptree fp16flags;
  for (const auto &flag : split_string(
           flags_to_string<ze_device_fp_atomic_ext_flag_t>(p.fp16Flags),
           " | ")) {
    pt::ptree node;
    node.put("", flag);
    fp16flags.push_back(std::make_pair("", node));
  }
  tree.add_child("fp16Flags", fp16flags);
  pt::ptree fp32flags;
  for (const auto &flag : split_string(
           flags_to_string<ze_device_fp_atomic_ext_flag_t>(p.fp32Flags),
           " | ")) {
    pt::ptree node;
    node.put("", flag);
    fp32flags.push_back(std::make_pair("", node));
  }
  tree.add_child("fp32Flags", fp32flags);
  pt::ptree fp64flags;
  for (const auto &flag : split_string(
           flags_to_string<ze_device_fp_atomic_ext_flag_t>(p.fp64Flags),
           " | ")) {
    pt::ptree node;
    node.put("", flag);
    fp64flags.push_back(std::make_pair("", node));
  }
  tree.add_child("fp64Flags", fp64flags);
  return tree;
}

boost::property_tree::ptree device_raytracing_properties_to_json(
    const ze_device_raytracing_ext_properties_t &p) {
  pt::ptree tree;
  pt::ptree flags_node;
  for (const auto &flag : split_string(
           flags_to_string<ze_device_raytracing_ext_flag_t>(p.flags), " | ")) {
    pt::ptree node;
    node.put("", flag);
    flags_node.push_back(std::make_pair("", node));
  }
  tree.add_child("flags", flags_node);
  tree.put("maxBVHLevels", p.maxBVHLevels);
  return tree;
}

boost::property_tree::ptree
device_capabilities_to_json(const DeviceCapabilities &capabilities) {
  pt::ptree tree;
  tree.add_child("ze_device_properties_t",
                 device_properties_to_json(capabilities.device_properties));
  tree.add_child(
      "ze_device_compute_properties_t",
      device_compute_properties_to_json(capabilities.compute_properties));
  tree.add_child(
      "ze_device_module_properties_t",
      device_module_properties_to_json(capabilities.module_properties));
  tree.add_child("ze_command_queue_group_properties_t",
                 all_device_command_queue_group_properties_to_json(
                     capabilities.command_queue_group_properties));
  tree.add_child("ze_scheduling_hint_exp_properties_t",
                 kernel_scheduling_hint_properties_to_json(
                     capabilities.scheduling_hint_properties));
  tree.add_child(
      "ze_float_atomic_ext_properties_t",
      float_atomics_properties_to_json(capabilities.float_atomics_properties));
  tree.add_child("ze_device_raytracing_ext_properties_t",
                 device_raytracing_properties_to_json(
                     capabilities.ray_tracing_properties));
  tree.add_child(
      "ze_device_memory_properties_t",
      all_device_memory_properties_to_json(capabilities.memory_properties));
  tree.add_child("ze_device_memory_access_properties_t",
                 device_memory_access_properties_to_json(
                     capabilities.memory_access_properties));
  tree.add_child(
      "ze_device_cache_properties_t",
      all_device_cache_properties_to_json(capabilities.cache_properties));
  tree.add_child(
      "ze_device_image_properties_t",
      device_image_properties_to_json(capabilities.image_properties));
  tree.add_child("ze_device_external_memory_properties_t",
                 device_external_memory_properties_to_json(
                     capabilities.external_memory_properties));
  tree.add_child(
      "zet_device_debug_properties_t",
      device_debug_properties_to_json(capabilities.debug_properties));
  tree.add_child("ze_mutable_command_list_exp_properties_t",
                 device_mutable_command_list_properties_to_json(
                     capabilities.mutable_command_list_properties));
  tree.add_child("zet_metric_programmable_exp_properties_t",
                 programmable_metrics_properties_to_json(
                     capabilities.programmable_metrics_properties));
  tree.add_child("zet_metric_group_properties_t",
                 tracer_metrics_properties_to_json(
                     capabilities.tracer_metrics_properties));
  tree.put("programmable_metrics_count",
           capabilities.programmable_metrics_count);
  tree.put("zes_engine_handle_t_count",
           std::to_string(capabilities.sysman_engine_properties.size()));
  tree.add_child("zes_engine_properties_t",
                 all_device_sysman_properties_to_json(
                     capabilities.sysman_engine_properties));
  tree.put("zes_diag_handle_t_count",
           std::to_string(capabilities.sysman_diagnostic_properties.size()));
  tree.add_child("zes_diag_properties_t",
                 all_device_sysman_properties_to_json(
                     capabilities.sysman_diagnostic_properties));
  tree.put("zes_mem_handle_t_count",
           std::to_string(capabilities.sysman_memory_properties.size()));
  tree.add_child("zes_mem_properties_t",
                 all_device_sysman_properties_to_json(
                     capabilities.sysman_memory_properties));
  tree.put("zes_pwr_handle_t_count",
           std::to_string(capabilities.sysman_power_properties.size()));
  tree.add_child("zes_power_properties_t",
                 all_device_sysman_properties_to_json(
                     capabilities.sysman_power_properties));
  tree.put("zes_freq_handle_t_count",
           std::to_string(capabilities.sysman_frequency_properties.size()));
  tree.add_child("zes_freq_properties_t",
                 all_device_sysman_properties_to_json(
                     capabilities.sysman_frequency_properties));
  tree.put("zes_temp_handle_t_count",
           std::to_string(capabilities.sysman_temperature_properties.size()));
  tree.add_child("zes_temp_properties_t",
                 all_device_sysman_properties_to_json(
                     capabilities.sysman_temperature_properties));
  tree.put("zes_ras_handle_t_count",
           std::to_string(capabilities.sysman_ras_handles_count));
  tree.put("zes_vf_handle_t_count",
           std::to_string(capabilities.sysman_vf_handles_count));
  tree.put("zes_perf_handle_t_count",
           std::to_string(capabilities.sysman_performance_handles_count));
  tree.put("zes_firmware_handle_t_count",
           std::to_string(capabilities.sysman_firmware_handles_count));
  pt::ptree sub_devices;
  tree.put("sub_devices_count",
           std::to_string(capabilities.sub_devices.size()));
  if (capabilities.sub_devices.empty()) {
    pt::ptree node;
    sub_devices.push_back(std::make_pair("", node));
  } else {
    for (const auto &sub_device : capabilities.sub_devices) {
      sub_devices.push_back(
          std::make_pair("", device_capabilities_to_json(sub_device)));
    }
  }
  tree.add_child("sub_devices", sub_devices);
  return tree;
}

boost::property_tree::ptree
device_properties_to_json(const ze_device_properties_t &p) {
  pt::ptree tree;
  tree.put("name", p.name);
  tree.put("type", to_string(p.type));
  tree.put("vendorId", p.vendorId);
  tree.put("deviceId", p.deviceId);
  tree.put("uuid", uuid_to_string(p.uuid.id));
  pt::ptree flags_node;
  for (const auto &flag : split_string(
           flags_to_string<ze_device_property_flag_t>(p.flags), " | ")) {
    pt::ptree node;
    node.put("", flag);
    flags_node.push_back(std::make_pair("", node));
  }
  tree.add_child("flags", flags_node);
  if ((p.flags & ZE_DEVICE_PROPERTY_FLAG_SUBDEVICE) != 0u) {
    tree.put("subdeviceId", p.subdeviceId);
  }
  tree.put("coreClockRate", p.coreClockRate);
  tree.put("maxMemAllocSize", p.maxMemAllocSize);
  tree.put("maxHardwareContexts", p.maxHardwareContexts);
  tree.put("maxCommandQueuePriority", p.maxCommandQueuePriority);
  tree.put("numThreadsPerEU", p.numThreadsPerEU);
  tree.put("physicalEUSimdWidth", p.physicalEUSimdWidth);
  tree.put("numEUsPerSubslice", p.numEUsPerSubslice);
  tree.put("numSubslicesPerSlice", p.numSubslicesPerSlice);
  tree.put("numSlices", p.numSlices);
  tree.put("timerResolution", p.timerResolution);
  tree.put("timestampValidBits", p.timestampValidBits);
  tree.put("kernelTimestampValidBits", p.kernelTimestampValidBits);
  return tree;
}

boost::property_tree::ptree
device_compute_properties_to_json(const ze_device_compute_properties_t &p) {
  pt::ptree tree;
  tree.put("maxTotalGroupSize", p.maxTotalGroupSize);
  tree.put("maxGroupSizeX", p.maxGroupSizeX);
  tree.put("maxGroupSizeY", p.maxGroupSizeY);
  tree.put("maxGroupSizeZ", p.maxGroupSizeZ);
  tree.put("maxGroupCountX", p.maxGroupCountX);
  tree.put("maxGroupCountY", p.maxGroupCountY);
  tree.put("maxGroupCountZ", p.maxGroupCountZ);
  tree.put("maxSharedLocalMemory", p.maxSharedLocalMemory);
  pt::ptree sub_group_sizes;
  if (p.numSubGroupSizes == 0) {
    pt::ptree node;
    sub_group_sizes.push_back(std::make_pair("", node));
  } else {
    for (uint32_t i = 0; i < p.numSubGroupSizes; ++i) {
      pt::ptree node;
      node.put("", p.subGroupSizes[i]);
      sub_group_sizes.push_back(std::make_pair("", node));
    }
  }
  tree.add_child("subGroupSizes", sub_group_sizes);
  return tree;
}

boost::property_tree::ptree
device_module_properties_to_json(const ze_device_module_properties_t &p) {
  pt::ptree tree;
  const auto int_version = p.spirvVersionSupported;
  const std::string string_version =
      std::to_string(ZE_MAJOR_VERSION(int_version)) + "." +
      std::to_string(ZE_MINOR_VERSION(int_version));
  tree.put("spirvVersionSupported", string_version);
  pt::ptree flags_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_device_module_flag_t>(p.flags), " | ")) {
    pt::ptree node;
    node.put("", flag);
    flags_node.push_back(std::make_pair("", node));
  }
  tree.add_child("flags", flags_node);
  pt::ptree fp16flags_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_device_fp_flag_t>(p.fp16flags), " | ")) {
    pt::ptree node;
    node.put("", flag);
    fp16flags_node.push_back(std::make_pair("", node));
  }
  tree.add_child("fp16flags", fp16flags_node);
  pt::ptree fp32flags_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_device_fp_flag_t>(p.fp32flags), " | ")) {
    pt::ptree node;
    node.put("", flag);
    fp32flags_node.push_back(std::make_pair("", node));
  }
  tree.add_child("fp32flags", fp32flags_node);
  pt::ptree fp64flags_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_device_fp_flag_t>(p.fp64flags), " | ")) {
    pt::ptree node;
    node.put("", flag);
    fp64flags_node.push_back(std::make_pair("", node));
  }
  tree.add_child("fp64flags", fp64flags_node);
  tree.put("maxArgumentsSize", p.maxArgumentsSize);
  tree.put("printfBufferSize", p.printfBufferSize);
  tree.put("nativeKernelSupported", uuid_to_string(p.nativeKernelSupported.id));
  return tree;
}

boost::property_tree::ptree all_device_command_queue_group_properties_to_json(
    const std::vector<ze_command_queue_group_properties_t> &p) {
  pt::ptree tree;
  if (p.empty()) {
    pt::ptree node;
    tree.push_back(std::make_pair("", node));
  } else {
    for (const auto &properties : p) {
      tree.push_back(std::make_pair(
          "", device_command_queue_group_properties_to_json(properties)));
    }
  }
  return tree;
}

boost::property_tree::ptree device_command_queue_group_properties_to_json(
    const ze_command_queue_group_properties_t &p) {
  pt::ptree tree;
  pt::ptree flags_node;
  for (const auto &flag : split_string(
           flags_to_string<ze_command_queue_group_property_flag_t>(p.flags),
           " | ")) {
    pt::ptree node;
    node.put("", flag);
    flags_node.push_back(std::make_pair("", node));
  }
  tree.add_child("flags", flags_node);
  tree.put("maxMemoryFillPatternSize", p.maxMemoryFillPatternSize);
  tree.put("numQueues", p.numQueues);
  return tree;
}

boost::property_tree::ptree all_device_memory_properties_to_json(
    const std::vector<ze_device_memory_properties_t> &p) {
  pt::ptree tree;
  if (p.empty()) {
    pt::ptree node;
    tree.push_back(std::make_pair("", node));
  } else {
    for (const auto &properties : p) {
      tree.push_back(
          std::make_pair("", device_memory_properties_to_json(properties)));
    }
  }
  return tree;
}

boost::property_tree::ptree
device_memory_properties_to_json(const ze_device_memory_properties_t &p) {
  pt::ptree tree;
  tree.put("name", p.name);
  pt::ptree flags_node;
  for (const auto &flag : split_string(
           flags_to_string<ze_device_memory_property_flag_t>(p.flags), " | ")) {
    pt::ptree node;
    node.put("", flag);
    flags_node.push_back(std::make_pair("", node));
  }
  tree.add_child("flags", flags_node);
  tree.put("maxClockRate", p.maxClockRate);
  tree.put("maxBusWidth", p.maxBusWidth);
  tree.put("totalSize", p.totalSize);
  return tree;
}

boost::property_tree::ptree device_memory_access_properties_to_json(
    const ze_device_memory_access_properties_t &p) {
  pt::ptree tree;
  pt::ptree host_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_memory_access_cap_flag_t>(
                        p.hostAllocCapabilities),
                    " | ")) {
    pt::ptree node;
    node.put("", flag);
    host_node.push_back(std::make_pair("", node));
  }
  tree.add_child("hostAllocCapabilities", host_node);
  pt::ptree device_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_memory_access_cap_flag_t>(
                        p.deviceAllocCapabilities),
                    " | ")) {
    pt::ptree node;
    node.put("", flag);
    device_node.push_back(std::make_pair("", node));
  }
  tree.add_child("deviceAllocCapabilities", device_node);
  pt::ptree shared_single_device_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_memory_access_cap_flag_t>(
                        p.sharedSingleDeviceAllocCapabilities),
                    " | ")) {
    pt::ptree node;
    node.put("", flag);
    shared_single_device_node.push_back(std::make_pair("", node));
  }
  tree.add_child("sharedSingleDeviceAllocCapabilities",
                 shared_single_device_node);
  pt::ptree shared_cross_device_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_memory_access_cap_flag_t>(
                        p.sharedCrossDeviceAllocCapabilities),
                    " | ")) {
    pt::ptree node;
    node.put("", flag);
    shared_cross_device_node.push_back(std::make_pair("", node));
  }
  tree.add_child("sharedCrossDeviceAllocCapabilities",
                 shared_cross_device_node);
  pt::ptree system_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_memory_access_cap_flag_t>(
                        p.sharedSystemAllocCapabilities),
                    " | ")) {
    pt::ptree node;
    node.put("", flag);
    system_node.push_back(std::make_pair("", node));
  }
  tree.add_child("sharedSystemAllocCapabilities", system_node);
  return tree;
}

boost::property_tree::ptree all_device_cache_properties_to_json(
    const std::vector<ze_device_cache_properties_t> &p) {
  pt::ptree tree;
  if (p.empty()) {
    pt::ptree node;
    tree.push_back(std::make_pair("", node));
  } else {
    for (const auto &properties : p) {
      tree.push_back(
          std::make_pair("", device_cache_properties_to_json(properties)));
    }
  }
  return tree;
}

boost::property_tree::ptree
device_cache_properties_to_json(const ze_device_cache_properties_t &p) {
  pt::ptree tree;
  pt::ptree flags_node;
  for (const auto &flag : split_string(
           flags_to_string<ze_device_cache_property_flag_t>(p.flags), " | ")) {
    pt::ptree node;
    node.put("", flag);
    flags_node.push_back(std::make_pair("", node));
  }
  tree.add_child("flags", flags_node);
  tree.put("cacheSize", p.cacheSize);
  return tree;
}

boost::property_tree::ptree
device_image_properties_to_json(const ze_device_image_properties_t &p) {
  pt::ptree tree;
  tree.put("maxImageDims1D", p.maxImageDims1D);
  tree.put("maxImageDims2D", p.maxImageDims2D);
  tree.put("maxImageDims3D", p.maxImageDims3D);
  tree.put("maxImageBufferSize", p.maxImageBufferSize);
  tree.put("maxImageArraySlices", p.maxImageArraySlices);
  tree.put("maxSamplers", p.maxSamplers);
  tree.put("maxReadImageArgs", p.maxReadImageArgs);
  tree.put("maxWriteImageArgs", p.maxWriteImageArgs);
  return tree;
}

boost::property_tree::ptree device_external_memory_properties_to_json(
    const ze_device_external_memory_properties_t &p) {
  pt::ptree tree;
  pt::ptree memory_import_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_external_memory_type_flag_t>(
                        p.memoryAllocationImportTypes),
                    " | ")) {
    pt::ptree node;
    node.put("", flag);
    memory_import_node.push_back(std::make_pair("", node));
  }
  tree.add_child("memoryAllocationImportTypes", memory_import_node);
  pt::ptree memory_export_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_external_memory_type_flag_t>(
                        p.memoryAllocationExportTypes),
                    " | ")) {
    pt::ptree node;
    node.put("", flag);
    memory_export_node.push_back(std::make_pair("", node));
  }
  tree.add_child("memoryAllocationExportTypes", memory_export_node);
  pt::ptree image_import_node;
  for (const auto &flag : split_string(
           flags_to_string<ze_external_memory_type_flag_t>(p.imageImportTypes),
           " | ")) {
    pt::ptree node;
    node.put("", flag);
    image_import_node.push_back(std::make_pair("", node));
  }
  tree.add_child("imageImportTypes", image_import_node);
  pt::ptree image_export_node;
  for (const auto &flag : split_string(
           flags_to_string<ze_external_memory_type_flag_t>(p.imageExportTypes),
           " | ")) {
    pt::ptree node;
    node.put("", flag);
    image_export_node.push_back(std::make_pair("", node));
  }
  tree.add_child("imageExportTypes", image_export_node);
  return tree;
}

boost::property_tree::ptree
device_debug_properties_to_json(const zet_device_debug_properties_t &p) {
  pt::ptree tree;
  pt::ptree debug_properties_node;
  for (const auto &flag : split_string(
           flags_to_string<zet_device_debug_property_flag_t>(p.flags), " | ")) {
    pt::ptree node;
    node.put("", flag);
    debug_properties_node.push_back(std::make_pair("", node));
  }
  tree.add_child("flags", debug_properties_node);
  return tree;
}

boost::property_tree::ptree device_mutable_command_list_properties_to_json(
    const ze_mutable_command_list_exp_properties_t &p) {
  pt::ptree tree;
  pt::ptree mutable_command_list_node;
  pt::ptree mutable_command_node;
  for (const auto &flag :
       split_string(flags_to_string<ze_mutable_command_list_exp_flags_t>(
                        p.mutableCommandListFlags),
                    " | ")) {
    pt::ptree node;
    node.put("", flag);
    mutable_command_list_node.push_back(std::make_pair("", node));
  }
  tree.add_child("mutableCommandListFlags", mutable_command_list_node);

  for (const auto &flag :
       split_string(flags_to_string<ze_mutable_command_exp_flag_t>(
                        p.mutableCommandFlags),
                    " | ")) {
    pt::ptree node;
    node.put("", flag);
    mutable_command_node.push_back(std::make_pair("", node));
  }
  tree.add_child("mutableCommandFlags", mutable_command_node);
  return tree;
}

boost::property_tree::ptree programmable_metrics_properties_to_json(
    const std::vector<zet_metric_programmable_exp_properties_t> &properties) {
  pt::ptree array;
  for (const auto &p : properties) {
    pt::ptree tree;
    tree.put("name", p.name);
    tree.put("description", p.description);
    tree.put("component", p.component);
    tree.put("tierNumber", p.tierNumber);
    tree.put("domain", p.domain);
    tree.put("parameterCount", p.parameterCount);
    tree.put(
        "samplingType",
        flags_to_string<zet_metric_group_sampling_type_flag_t>(p.samplingType));
    tree.put("sourceId", p.sourceId);
    array.push_back(std::make_pair("", tree));
  }
  return array;
}

boost::property_tree::ptree tracer_metrics_properties_to_json(
    const std::vector<zet_metric_group_properties_t> &properties) {
  pt::ptree array;
  for (const auto &p : properties) {
    pt::ptree tree;
    tree.put("name", p.name);
    tree.put("description", p.description);
    tree.put(
        "samplingType",
        flags_to_string<zet_metric_group_sampling_type_flag_t>(p.samplingType));
    tree.put("domain", p.domain);
    tree.put("metricCount", p.metricCount);
    array.push_back(std::make_pair("", tree));
  }
  return array;
}

template <typename PROPERTIES>
boost::property_tree::ptree
all_device_sysman_properties_to_json(const std::vector<PROPERTIES> &p) {
  pt::ptree tree;
  for (const auto &properties : p) {
    if (properties.stype == ZES_STRUCTURE_TYPE_FORCE_UINT32) {
      continue;
    }
    tree.push_back(
        std::make_pair("", device_sysman_properties_to_json(properties)));
  }
  if (tree.empty()) {
    pt::ptree node;
    tree.push_back(std::make_pair("", node));
  }
  return tree;
}

boost::property_tree::ptree
device_sysman_properties_to_json(const zes_engine_properties_t &p) {
  pt::ptree tree;
  tree.put("type", to_string(p.type));
  tree.put("onSubdevice", to_string(p.onSubdevice));
  tree.put("subdeviceId", p.subdeviceId);
  return tree;
}

boost::property_tree::ptree
device_sysman_properties_to_json(const zes_power_properties_t &p) {
  pt::ptree tree;
  tree.put("canControl", to_string(p.canControl));
  tree.put("isEnergyThresholdSupported",
           to_string(p.isEnergyThresholdSupported));
  tree.put("onSubdevice", to_string(p.onSubdevice));
  tree.put("subdeviceId", p.subdeviceId);
  return tree;
}

boost::property_tree::ptree
device_sysman_properties_to_json(const zes_diag_properties_t &p) {
  pt::ptree tree;
  tree.put("name", p.name);
  tree.put("haveTests", to_string(p.haveTests));
  tree.put("onSubdevice", to_string(p.onSubdevice));
  tree.put("subdeviceId", p.subdeviceId);
  return tree;
}

boost::property_tree::ptree
device_sysman_properties_to_json(const zes_mem_properties_t &p) {
  pt::ptree tree;
  tree.put("type", to_string(p.type));
  tree.put("location", to_string(p.location));
  tree.put("physicalSize", p.physicalSize);
  tree.put("busWidth", p.busWidth);
  tree.put("numChannels", p.numChannels);
  tree.put("onSubdevice", to_string(p.onSubdevice));
  tree.put("subdeviceId", p.subdeviceId);
  return tree;
}

boost::property_tree::ptree
device_sysman_properties_to_json(const zes_freq_properties_t &p) {
  pt::ptree tree;
  tree.put("type", to_string(p.type));
  tree.put("canControl", to_string(p.canControl));
  tree.put("isThrottleEventSupported", to_string(p.isThrottleEventSupported));
  tree.put("min", p.min);
  tree.put("max", p.max);
  tree.put("onSubdevice", to_string(p.onSubdevice));
  tree.put("subdeviceId", p.subdeviceId);
  return tree;
}

boost::property_tree::ptree
device_sysman_properties_to_json(const zes_temp_properties_t &p) {
  pt::ptree tree;
  tree.put("type", to_string(p.type));
  tree.put("maxTemperature", p.maxTemperature);
  tree.put("isCriticalTempSupported", to_string(p.isCriticalTempSupported));
  tree.put("isThreshold1Supported", to_string(p.isThreshold1Supported));
  tree.put("isThreshold2Supported", to_string(p.isThreshold2Supported));
  tree.put("onSubdevice", to_string(p.onSubdevice));
  tree.put("subdeviceId", p.subdeviceId);
  return tree;
}

std::string ptree_to_string(const pt::ptree tree) {
  bool is_array = true;
  for (const pt::ptree::value_type &element : tree.get_child("")) {
    if (!element.first.empty()) {
      is_array = false;
      break;
    }
  }

  // Boost.PropertyTree does not support arrays as root elements -
  // https://stackoverflow.com/questions/2114466/creating-json-arrays-in-boost-using-property-trees#comment84042861_2114567
  std::stringstream ss;
  if (is_array) {
    pt::json_parser::write_json_helper(ss, tree, 1, true);
  } else {
    pt::write_json(ss, tree);
  }
  std::string output = ss.str();

  std::regex reg;
  // Boost.PropertyTree does not support empty arrays -
  // https://stackoverflow.com/a/44463046
  reg = std::regex("\\[\n.*\"\"\n.*\\]");
  output = std::regex_replace(output, reg, "[]");

  // Boost.PropertyTree does not support other JSON types than strings -
  // https://svn.boost.org/trac10/ticket/9721
  reg = std::regex(R"(\"([0-9]+)\")");
  output = std::regex_replace(output, reg, "$1");
  reg = std::regex(R"(\"(true|false)\")");
  output = std::regex_replace(output, reg, "$1");

  // Boost.PropertyTree adds a new line to the end of a string -
  // https://svn.boost.org/trac10/ticket/12149
  reg = std::regex("\\\n$");
  output = std::regex_replace(output, reg, "");

  // Fix for indentation after calling write_json_helper
  const std::string pattern = "    ]";
  const size_t position = output.rfind(pattern);
  if (position == output.size() - pattern.size()) {
    output = output.erase(position, pattern.size() - 1);
  }

  return output;
}

void write_capabilities_to_json(
    const std::vector<DriverCapabilities> &capabilities,
    const std::string output) {
  const pt::ptree tree = drivers_capabilities_to_json(capabilities);
  const std::string data = ptree_to_string(tree);
  save_text_file(data, output);
}

} // namespace compute_samples
