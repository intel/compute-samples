/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ze_info/text_formatter.hpp"

#include "ze_utils/ze_utils.hpp"

#include <sstream>
#include <string>
#include <iomanip>

namespace compute_samples {

std::string
drivers_capabilities_to_text(const std::vector<DriverCapabilities> capabilities,
                             const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Number of drivers",
                          std::to_string(capabilities.size()),
                          indentation_level);
  for (const auto &capability : capabilities) {
    ss << driver_capabilities_to_text(capability, indentation_level + 1);
  }
  return ss.str();
}

std::string driver_capabilities_to_text(const DriverCapabilities capabilities,
                                        const int indentation_level) {
  std::stringstream ss;
  ss << driver_api_version_to_text(capabilities.api_version, indentation_level);
  ss << driver_properties_to_text(capabilities.driver_properties,
                                  indentation_level);
  ss << driver_ipc_properties_to_text(capabilities.ipc_properties,
                                      indentation_level);
  ss << key_value_to_text("Number of devices",
                          std::to_string(capabilities.devices.size()),
                          indentation_level);
  for (const auto &device : capabilities.devices) {
    ss << device_capabilities_to_text(device, indentation_level + 1);
  }
  return ss.str();
}

std::string driver_api_version_to_text(const ze_api_version_t v,
                                       const int indentation_level) {
  std::stringstream ss;
  const std::string version = std::to_string(ZE_MAJOR_VERSION(v)) + "." +
                              std::to_string(ZE_MINOR_VERSION(v));
  ss << key_value_to_text("Driver API version", version, indentation_level);
  return ss.str();
}

std::string driver_properties_to_text(const ze_driver_properties_t p,
                                      const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Driver version", std::to_string(p.driverVersion),
                          indentation_level);
  ss << key_value_to_text("UUID", to_string(p.uuid), indentation_level);
  return ss.str();
}

std::string driver_ipc_properties_to_text(const ze_driver_ipc_properties_t p,
                                          const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("IPC memory supported",
                          p.memsSupported != 0u ? "true" : "false",
                          indentation_level);
  ss << key_value_to_text("IPC events supported",
                          p.eventsSupported != 0u ? "true" : "false",
                          indentation_level);
  return ss.str();
}

std::string device_capabilities_to_text(const DeviceCapabilities capabilities,
                                        const int indentation_level) {
  std::stringstream ss;
  ss << device_properties_to_text(capabilities.device_properties,
                                  indentation_level);
  ss << device_compute_properties_to_text(capabilities.compute_properties,
                                          indentation_level);
  ss << device_kernel_properties_to_text(capabilities.kernel_properties,
                                         indentation_level);
  ss << all_device_memory_properties_to_text(capabilities.memory_properties,
                                             indentation_level);
  ss << device_memory_access_properties_to_text(
      capabilities.memory_access_properties, indentation_level);
  ss << device_cache_properties_to_text(capabilities.cache_properties,
                                        indentation_level);
  ss << device_image_properties_to_text(capabilities.image_properties,
                                        indentation_level);
  ss << key_value_to_text("Number of sub-devices",
                          std::to_string(capabilities.sub_devices.size()),
                          indentation_level);
  for (const auto &sub_device : capabilities.sub_devices) {
    ss << device_capabilities_to_text(sub_device, indentation_level + 1);
  }
  return ss.str();
}

std::string device_properties_to_text(const ze_device_properties_t p,
                                      const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Device name", p.name, indentation_level);
  ss << key_value_to_text("Device type", to_string(p.type), indentation_level);
  ss << key_value_to_text("Vendor ID", std::to_string(p.vendorId),
                          indentation_level);
  ss << key_value_to_text("Device ID", std::to_string(p.deviceId),
                          indentation_level);
  ss << key_value_to_text("UUID", to_string(p.uuid), indentation_level);
  ss << key_value_to_text("Is sub-device",
                          p.isSubdevice != 0u ? "true" : "false",
                          indentation_level);
  if (p.isSubdevice == 1u) {
    ss << key_value_to_text("Sub-device ID", std::to_string(p.subdeviceId),
                            indentation_level);
  }
  ss << key_value_to_text("Core clock rate", std::to_string(p.coreClockRate),
                          indentation_level);
  ss << key_value_to_text("Is unified memory supported",
                          p.unifiedMemorySupported != 0u ? "true" : "false",
                          indentation_level);
  ss << key_value_to_text("Is ECC memory supported",
                          p.eccMemorySupported != 0u ? "true" : "false",
                          indentation_level);
  ss << key_value_to_text("Are on demand page faults supported",
                          p.onDemandPageFaultsSupported != 0u ? "true"
                                                              : "false",
                          indentation_level);
  ss << key_value_to_text("Max command queues",
                          std::to_string(p.maxCommandQueues),
                          indentation_level);
  ss << key_value_to_text("Number of asynchronous compute engines",
                          std::to_string(p.numAsyncComputeEngines),
                          indentation_level);
  ss << key_value_to_text("Number of asynchronous copy engines",
                          std::to_string(p.numAsyncCopyEngines),
                          indentation_level);
  ss << key_value_to_text("Max command queue priority",
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
  return ss.str();
}

std::string
device_compute_properties_to_text(const ze_device_compute_properties_t p,
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
device_kernel_properties_to_text(const ze_device_kernel_properties_t p,
                                 const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("SPIR-V version",
                          std::to_string(p.spirvVersionSupported),
                          indentation_level);
  ss << key_value_to_text("Native kernel supported",
                          to_string(p.nativeKernelSupported),
                          indentation_level);
  ss << key_value_to_text("FP16 supported",
                          p.fp16Supported != 0u ? "true" : "false",
                          indentation_level);
  ss << key_value_to_text("FP64 supported",
                          p.fp64Supported != 0u ? "true" : "false",
                          indentation_level);
  ss << key_value_to_text("INT64 atomics supported",
                          p.int64AtomicsSupported != 0u ? "true" : "false",
                          indentation_level);
  ss << key_value_to_text("DP4A supported",
                          p.dp4aSupported != 0u ? "true" : "false",
                          indentation_level);
  ss << key_value_to_text("Half-precision floating-point capabilities",
                          to_string(p.halfFpCapabilities), indentation_level);
  ss << key_value_to_text("Single-precision floating-point capabilities",
                          to_string(p.singleFpCapabilities), indentation_level);
  ss << key_value_to_text("Double-precision floating-point capabilities",
                          to_string(p.doubleFpCapabilities), indentation_level);
  ss << key_value_to_text("Max argument size",
                          std::to_string(p.maxArgumentsSize),
                          indentation_level);
  ss << key_value_to_text("printf buffer size",
                          std::to_string(p.printfBufferSize),
                          indentation_level);
  return ss.str();
}

std::string all_device_memory_properties_to_text(
    const std::vector<ze_device_memory_properties_t> &p,
    const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Number of memory properties",
                          std::to_string(p.size()), indentation_level);
  for (const auto &properties : p) {
    ss << device_memory_properties_to_text(properties, indentation_level + 1);
  }
  return ss.str();
}

std::string
device_memory_properties_to_text(const ze_device_memory_properties_t p,
                                 const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Max clock rate", std::to_string(p.maxClockRate),
                          indentation_level);
  ss << key_value_to_text("Max bus width", std::to_string(p.maxBusWidth),
                          indentation_level);
  ss << key_value_to_text("Total size", std::to_string(p.totalSize),
                          indentation_level);
  return ss.str();
}

std::string device_memory_access_properties_to_text(
    const ze_device_memory_access_properties_t p, const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Host allocation capabilities",
                          to_string(p.hostAllocCapabilities),
                          indentation_level);
  ss << key_value_to_text("Device allocation capabilities",
                          to_string(p.deviceAllocCapabilities),
                          indentation_level);
  ss << key_value_to_text("Shared single-device allocation capabilities",
                          to_string(p.sharedSingleDeviceAllocCapabilities),
                          indentation_level);
  ss << key_value_to_text("Shared cross-device allocation capabilities",
                          to_string(p.sharedCrossDeviceAllocCapabilities),
                          indentation_level);
  ss << key_value_to_text("Shared system allocation capabilities",
                          to_string(p.sharedSystemAllocCapabilities),
                          indentation_level);
  return ss.str();
}

std::string
device_cache_properties_to_text(const ze_device_cache_properties_t p,
                                const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Intermediate cache control supported",
                          p.intermediateCacheControlSupported != 0u ? "true"
                                                                    : "false",
                          indentation_level);
  ss << key_value_to_text("Intermediate cache size",
                          std::to_string(p.intermediateCacheSize),
                          indentation_level);
  ss << key_value_to_text("Intermediate cacheline size",
                          std::to_string(p.intermediateCachelineSize),
                          indentation_level);
  ss << key_value_to_text("Last level cache size control supported",
                          p.lastLevelCacheSizeControlSupported != 0u ? "true"
                                                                     : "false",
                          indentation_level);
  ss << key_value_to_text("Last level cache size",
                          std::to_string(p.lastLevelCacheSize),
                          indentation_level);
  ss << key_value_to_text("Last level cacheline size",
                          std::to_string(p.lastLevelCachelineSize),
                          indentation_level);
  return ss.str();
}

std::string
device_image_properties_to_text(const ze_device_image_properties_t p,
                                const int indentation_level) {
  std::stringstream ss;
  ss << key_value_to_text("Are images supported",
                          p.supported != 0u ? "true" : "false",
                          indentation_level);
  ss << key_value_to_text("Max image 1D", std::to_string(p.maxImageDims1D),
                          indentation_level);
  ss << key_value_to_text("Max image 2D", std::to_string(p.maxImageDims2D),
                          indentation_level);
  ss << key_value_to_text("Max image 3D", std::to_string(p.maxImageDims3D),
                          indentation_level);
  ss << key_value_to_text("Max image buffer slices",
                          std::to_string(p.maxImageBufferSize),
                          indentation_level);
  ss << key_value_to_text("Max image array slices",
                          std::to_string(p.maxImageArraySlices),
                          indentation_level);
  ss << key_value_to_text("Max number of samplers",
                          std::to_string(p.maxSamplers), indentation_level);
  ss << key_value_to_text("Max number of read arguments",
                          std::to_string(p.maxReadImageArgs),
                          indentation_level);
  ss << key_value_to_text("Max number of write arguments",
                          std::to_string(p.maxWriteImageArgs),
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
