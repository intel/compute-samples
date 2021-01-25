/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ze_info/text_formatter.hpp"
#include "utils/utils.hpp"
#include "ze_utils/ze_utils.hpp"
#include "ze_api.h"

#include <sstream>
#include <string>
#include <iomanip>

namespace compute_samples {

std::string drivers_capabilities_to_text(
    const std::vector<DriverCapabilities> &capabilities,
    const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Number of drivers",
                          std::to_string(capabilities.size()),
                          indentation_level);
  for (size_t i = 0; i < capabilities.size(); ++i) {
    ss << key_value_to_text("Driver", std::to_string(i), indentation_level);
    ss << driver_capabilities_to_text(capabilities[i], indentation_level + 1);
  }
  return ss.str();
}

std::string driver_capabilities_to_text(const DriverCapabilities &capabilities,
                                        const int indentation_level) {
  std::stringstream ss;
  ss << driver_api_version_to_text(capabilities.api_version, indentation_level);
  ss << driver_properties_to_text(capabilities.driver_properties,
                                  indentation_level);
  ss << driver_ipc_properties_to_text(capabilities.ipc_properties,
                                      indentation_level);
  ss << all_driver_extension_properties_to_text(
      capabilities.extension_properties, indentation_level);
  ss << key_value_to_text("Number of devices",
                          std::to_string(capabilities.devices.size()),
                          indentation_level);
  for (size_t i = 0; i < capabilities.devices.size(); ++i) {
    ss << key_value_to_text("Device", std::to_string(i), indentation_level);
    ss << device_capabilities_to_text(capabilities.devices[i],
                                      indentation_level + 1);
  }
  return ss.str();
}

std::string driver_api_version_to_text(const ze_api_version_t &v,
                                       const int indentation_level) {
  std::stringstream ss;
  const auto int_version = static_cast<uint32_t>(v);
  const std::string string_version =
      std::to_string(ZE_MAJOR_VERSION(int_version)) + "." +
      std::to_string(ZE_MINOR_VERSION(int_version));
  ss << key_value_to_text("Driver API version", string_version,
                          indentation_level);
  return ss.str();
}

std::string driver_properties_to_text(const ze_driver_properties_t &p,
                                      const int indentation_level) {

  std::stringstream ss;
  ss << key_value_to_text("Driver version", std::to_string(p.driverVersion),
                          indentation_level);
  ss << key_value_to_text("UUID", uuid_to_string(p.uuid.id), indentation_level);
  return ss.str();
}

std::string driver_ipc_properties_to_text(const ze_driver_ipc_properties_t &p,
                                          const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("IPC flags",
                          flags_to_string<ze_ipc_property_flag_t>(p.flags),
                          indentation_level);
  return ss.str();
}

std::string all_driver_extension_properties_to_text(
    const std::vector<ze_driver_extension_properties_t> &p,
    int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Number of extensions", std::to_string(p.size()),
                          indentation_level);
  for (size_t i = 0; i < p.size(); ++i) {
    ss << key_value_to_text("Extension", std::to_string(i), indentation_level);
    ss << driver_extension_properties_to_text(p[i], indentation_level + 1);
  }
  return ss.str();
}

std::string
driver_extension_properties_to_text(const ze_driver_extension_properties_t &p,
                                    int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Name", p.name, indentation_level);
  const auto int_version = p.version;
  const std::string string_version =
      std::to_string(ZE_MAJOR_VERSION(int_version)) + "." +
      std::to_string(ZE_MINOR_VERSION(int_version));
  ss << key_value_to_text("Version", string_version, indentation_level);
  return ss.str();
}

std::string device_capabilities_to_text(const DeviceCapabilities &capabilities,
                                        const int indentation_level) {
  std::stringstream ss;
  ss << device_properties_to_text(capabilities.device_properties,
                                  indentation_level);
  ss << device_compute_properties_to_text(capabilities.compute_properties,
                                          indentation_level);
  ss << device_module_properties_to_text(capabilities.module_properties,
                                         indentation_level);
  ss << all_device_command_queue_group_properties_to_text(
      capabilities.command_queue_group_properties, indentation_level);
  ss << all_device_memory_properties_to_text(capabilities.memory_properties,
                                             indentation_level);
  ss << device_memory_access_properties_to_text(
      capabilities.memory_access_properties, indentation_level);
  ss << all_device_cache_properties_to_text(capabilities.cache_properties,
                                            indentation_level);
  ss << device_image_properties_to_text(capabilities.image_properties,
                                        indentation_level);
  ss << device_external_memory_properties_to_text(
      capabilities.external_memory_properties, indentation_level);
  ss << key_value_to_text("Number of sub-devices",
                          std::to_string(capabilities.sub_devices.size()),
                          indentation_level);
  for (size_t i = 0; i < capabilities.sub_devices.size(); ++i) {
    ss << key_value_to_text("Sub-device", std::to_string(i), indentation_level);
    ss << device_capabilities_to_text(capabilities.sub_devices[i],
                                      indentation_level + 1);
  }
  return ss.str();
}

std::string device_properties_to_text(const ze_device_properties_t &p,
                                      const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Name", p.name, indentation_level);
  ss << key_value_to_text("Type", to_string(p.type), indentation_level);
  ss << key_value_to_text("Vendor ID", std::to_string(p.vendorId),
                          indentation_level);
  ss << key_value_to_text("Device ID", std::to_string(p.deviceId),
                          indentation_level);
  ss << key_value_to_text("UUID", uuid_to_string(p.uuid.id), indentation_level);
  ss << key_value_to_text("Device flags",
                          flags_to_string<ze_device_property_flag_t>(p.flags),
                          indentation_level);
  if ((p.flags & ZE_DEVICE_PROPERTY_FLAG_SUBDEVICE) != 0u) {
    ss << key_value_to_text("Sub-device ID", std::to_string(p.subdeviceId),
                            indentation_level);
  }
  ss << key_value_to_text("Core clock rate", std::to_string(p.coreClockRate),
                          indentation_level);
  ss << key_value_to_text("Maximum memory allocation size",
                          std::to_string(p.maxMemAllocSize), indentation_level);
  ss << key_value_to_text("Maximum number of logical hardware contexts",
                          std::to_string(p.maxHardwareContexts),
                          indentation_level);
  ss << key_value_to_text("Maximum priority for command queues",
                          std::to_string(p.maxCommandQueuePriority),
                          indentation_level);
  ss << key_value_to_text("Number of threads per EU",
                          std::to_string(p.numThreadsPerEU), indentation_level);
  ss << key_value_to_text("Physical EU SIMD width",
                          std::to_string(p.physicalEUSimdWidth),
                          indentation_level);
  ss << key_value_to_text("Number of EUs per subslice",
                          std::to_string(p.numEUsPerSubslice),
                          indentation_level);
  ss << key_value_to_text("Number of subslices per slice",
                          std::to_string(p.numSubslicesPerSlice),
                          indentation_level);
  ss << key_value_to_text("Number of slices", std::to_string(p.numSlices),
                          indentation_level);
  ss << key_value_to_text("Timer resolution", std::to_string(p.timerResolution),
                          indentation_level);
  ss << key_value_to_text("Timestamp valid bits",
                          std::to_string(p.timestampValidBits),
                          indentation_level);
  ss << key_value_to_text("Kernel timestamp valid bits",
                          std::to_string(p.kernelTimestampValidBits),
                          indentation_level);
  return ss.str();
}

std::string
device_compute_properties_to_text(const ze_device_compute_properties_t &p,
                                  const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Max total group size",
                          std::to_string(p.maxTotalGroupSize),
                          indentation_level);
  ss << key_value_to_text("Max group size X", std::to_string(p.maxGroupSizeX),
                          indentation_level);
  ss << key_value_to_text("Max group size Y", std::to_string(p.maxGroupSizeY),
                          indentation_level);
  ss << key_value_to_text("Max group size Z", std::to_string(p.maxGroupSizeZ),
                          indentation_level);
  ss << key_value_to_text("Max group count X", std::to_string(p.maxGroupCountX),
                          indentation_level);
  ss << key_value_to_text("Max group count Y", std::to_string(p.maxGroupCountY),
                          indentation_level);
  ss << key_value_to_text("Max group count Z", std::to_string(p.maxGroupCountZ),
                          indentation_level);
  ss << key_value_to_text("Max shared local memory",
                          std::to_string(p.maxSharedLocalMemory),
                          indentation_level);
  std::string sub_group_sizes;
  for (uint32_t i = 0; i < p.numSubGroupSizes; ++i) {
    sub_group_sizes.append(std::to_string(p.subGroupSizes[i]));
    if (i < p.numSubGroupSizes - 1) {
      sub_group_sizes.append(" ");
    }
  }
  ss << key_value_to_text("Sub-group sizes", sub_group_sizes,
                          indentation_level);
  return ss.str();
}

std::string
device_module_properties_to_text(const ze_device_module_properties_t &p,
                                 const int indentation_level) {
  std::stringstream ss;
  const auto int_version = p.spirvVersionSupported;
  const std::string string_version =
      std::to_string(ZE_MAJOR_VERSION(int_version)) + "." +
      std::to_string(ZE_MINOR_VERSION(int_version));
  ss << key_value_to_text("SPIR-V version", string_version, indentation_level);
  ss << key_value_to_text("Module flags",
                          flags_to_string<ze_device_module_flag_t>(p.flags),
                          indentation_level);
  ss << key_value_to_text("FP16 flags",
                          flags_to_string<ze_device_fp_flag_t>(p.fp16flags),
                          indentation_level);
  ss << key_value_to_text("FP32 flags",
                          flags_to_string<ze_device_fp_flag_t>(p.fp32flags),
                          indentation_level);
  ss << key_value_to_text("FP64 flags",
                          flags_to_string<ze_device_fp_flag_t>(p.fp64flags),
                          indentation_level);
  ss << key_value_to_text("Maximum argument size",
                          std::to_string(p.maxArgumentsSize),
                          indentation_level);
  ss << key_value_to_text("printf buffer size",
                          std::to_string(p.printfBufferSize),
                          indentation_level);
  ss << key_value_to_text("Native kernel supported",
                          uuid_to_string(p.nativeKernelSupported.id),
                          indentation_level);
  return ss.str();
}

std::string all_device_command_queue_group_properties_to_text(
    const std::vector<ze_command_queue_group_properties_t> &p,
    const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Number of command queue groups",
                          std::to_string(p.size()), indentation_level);
  for (size_t i = 0; i < p.size(); ++i) {
    ss << key_value_to_text("Command queue group", std::to_string(i),
                            indentation_level);
    ss << device_command_queue_group_properties_to_text(p[i],
                                                        indentation_level + 1);
  }
  return ss.str();
}

std::string device_command_queue_group_properties_to_text(
    const ze_command_queue_group_properties_t &p, const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text(
      "Command queue group flags",
      flags_to_string<ze_command_queue_group_property_flag_t>(p.flags),
      indentation_level);
  ss << key_value_to_text("Maximum fill pattern size",
                          std::to_string(p.maxMemoryFillPatternSize),
                          indentation_level);
  ss << key_value_to_text("Number of physical command queues",
                          std::to_string(p.numQueues), indentation_level);
  return ss.str();
}

std::string all_device_memory_properties_to_text(
    const std::vector<ze_device_memory_properties_t> &p,
    const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Number of memory properties",
                          std::to_string(p.size()), indentation_level);
  for (size_t i = 0; i < p.size(); ++i) {
    ss << key_value_to_text("Memory", std::to_string(i), indentation_level);
    ss << device_memory_properties_to_text(p[i], indentation_level + 1);
  }
  return ss.str();
}

std::string
device_memory_properties_to_text(const ze_device_memory_properties_t &p,
                                 const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Name", p.name, indentation_level);
  ss << key_value_to_text(
      "Memory flags",
      flags_to_string<ze_device_memory_property_flag_t>(p.flags),
      indentation_level);
  ss << key_value_to_text("Maximum clock rate", std::to_string(p.maxClockRate),
                          indentation_level);
  ss << key_value_to_text("Maximum bus width", std::to_string(p.maxBusWidth),
                          indentation_level);
  ss << key_value_to_text("Total size", std::to_string(p.totalSize),
                          indentation_level);
  return ss.str();
}

std::string device_memory_access_properties_to_text(
    const ze_device_memory_access_properties_t &p,
    const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text(
      "Host allocation capabilities",
      flags_to_string<ze_memory_access_cap_flag_t>(p.hostAllocCapabilities),
      indentation_level);
  ss << key_value_to_text(
      "Device allocation capabilities",
      flags_to_string<ze_memory_access_cap_flag_t>(p.deviceAllocCapabilities),
      indentation_level);
  ss << key_value_to_text("Shared single-device allocation capabilities",
                          flags_to_string<ze_memory_access_cap_flag_t>(
                              p.sharedSingleDeviceAllocCapabilities),
                          indentation_level);
  ss << key_value_to_text("Shared cross-device allocation capabilities",
                          flags_to_string<ze_memory_access_cap_flag_t>(
                              p.sharedCrossDeviceAllocCapabilities),
                          indentation_level);
  ss << key_value_to_text("Shared system allocation capabilities",
                          flags_to_string<ze_memory_access_cap_flag_t>(
                              p.sharedSystemAllocCapabilities),
                          indentation_level);
  return ss.str();
}

std::string all_device_cache_properties_to_text(
    const std::vector<ze_device_cache_properties_t> &p,
    const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Number of cache properties",
                          std::to_string(p.size()), indentation_level);
  for (size_t i = 0; i < p.size(); ++i) {
    ss << key_value_to_text("Cache", std::to_string(i), indentation_level);
    ss << device_cache_properties_to_text(p[i], indentation_level + 1);
  }
  return ss.str();
}

std::string
device_cache_properties_to_text(const ze_device_cache_properties_t &p,
                                const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text(
      "Cache flags", flags_to_string<ze_device_cache_property_flag_t>(p.flags),
      indentation_level);
  ss << key_value_to_text("Cache size", std::to_string(p.cacheSize),
                          indentation_level);
  return ss.str();
}

std::string
device_image_properties_to_text(const ze_device_image_properties_t &p,
                                const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Maximum image 1D", std::to_string(p.maxImageDims1D),
                          indentation_level);
  ss << key_value_to_text("Maximum image 2D", std::to_string(p.maxImageDims2D),
                          indentation_level);
  ss << key_value_to_text("Maximum image 3D", std::to_string(p.maxImageDims3D),
                          indentation_level);
  ss << key_value_to_text("Maximum image buffer size",
                          std::to_string(p.maxImageBufferSize),
                          indentation_level);
  ss << key_value_to_text("Maximum image array slices",
                          std::to_string(p.maxImageArraySlices),
                          indentation_level);
  ss << key_value_to_text("Maximum number of samplers",
                          std::to_string(p.maxSamplers), indentation_level);
  ss << key_value_to_text("Maximum number of read arguments",
                          std::to_string(p.maxReadImageArgs),
                          indentation_level);
  ss << key_value_to_text("Maximum number of write arguments",
                          std::to_string(p.maxWriteImageArgs),
                          indentation_level);
  return ss.str();
}

std::string device_external_memory_properties_to_text(
    const ze_device_external_memory_properties_t &p,
    const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Memory allocation import types",
                          flags_to_string<ze_external_memory_type_flag_t>(
                              p.memoryAllocationImportTypes),
                          indentation_level);
  ss << key_value_to_text("Memory allocation import types",
                          flags_to_string<ze_external_memory_type_flag_t>(
                              p.memoryAllocationExportTypes),
                          indentation_level);
  ss << key_value_to_text(
      "Image import types",
      flags_to_string<ze_external_memory_type_flag_t>(p.imageImportTypes),
      indentation_level);
  ss << key_value_to_text(
      "Image export types",
      flags_to_string<ze_external_memory_type_flag_t>(p.imageImportTypes),
      indentation_level);
  return ss.str();
}

std::string key_value_to_text(const std::string key, const std::string value,
                              const int indentation_level) {
  const int count = indentation_level * 4;
  const std::string indentation(count, ' ');
  std::stringstream ss;
  ss << std::left << std::setw(80) << indentation + key << value << '\n';
  return ss.str();
}

} // namespace compute_samples
