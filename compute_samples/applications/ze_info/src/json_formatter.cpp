/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ze_info/json_formatter.hpp"

#include "ze_utils/ze_utils.hpp"
#include "utils/utils.hpp"

#include <sstream>
#include <string>
#include <iomanip>
#include <regex>

#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;

namespace compute_samples {

pt::ptree drivers_capabilities_to_json(
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

pt::ptree driver_capabilities_to_json(const DriverCapabilities &capabilities) {
  pt::ptree tree;
  const std::string version =
      std::to_string(ZE_MAJOR_VERSION(capabilities.api_version)) + "." +
      std::to_string(ZE_MINOR_VERSION(capabilities.api_version));
  tree.put("ze_api_version_t", version);
  tree.add_child("ze_driver_properties_t",
                 driver_properties_to_json(capabilities.driver_properties));
  tree.add_child("ze_driver_ipc_properties_t",
                 driver_ipc_properties_to_json(capabilities.ipc_properties));
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

pt::ptree driver_properties_to_json(const ze_driver_properties_t p) {
  pt::ptree tree;
  tree.put("uuid", to_string(p.uuid));
  tree.put("driverVersion", p.driverVersion);
  return tree;
}

pt::ptree driver_ipc_properties_to_json(const ze_driver_ipc_properties_t p) {
  pt::ptree tree;
  tree.put("memsSupported", bool(p.memsSupported));
  tree.put("eventsSupported", bool(p.eventsSupported));
  return tree;
}

pt::ptree device_capabilities_to_json(const DeviceCapabilities &capabilities) {
  pt::ptree tree;
  tree.add_child("ze_device_properties_t",
                 device_properties_to_json(capabilities.device_properties));
  tree.add_child(
      "ze_device_compute_properties_t",
      device_compute_properties_to_json(capabilities.compute_properties));
  tree.add_child(
      "ze_device_kernel_properties_t",
      device_kernel_properties_to_json(capabilities.kernel_properties));
  tree.add_child(
      "ze_device_memory_properties_t",
      all_device_memory_properties_to_json(capabilities.memory_properties));
  tree.add_child("ze_device_memory_access_properties_t",
                 device_memory_access_properties_to_json(
                     capabilities.memory_access_properties));
  tree.add_child(
      "ze_device_cache_properties_t",
      device_cache_properties_to_json(capabilities.cache_properties));
  tree.add_child(
      "ze_device_image_properties_t",
      device_image_properties_to_json(capabilities.image_properties));
  pt::ptree sub_devices;
  if (capabilities.sub_devices.empty()) {
    pt::ptree node;
    sub_devices.push_back(std::make_pair("", node));
  } else {
    for (const auto &sub_device : capabilities.sub_devices) {
      sub_devices.push_back(
          std::make_pair("", device_capabilities_to_json(sub_device)));
    }
  }
  tree.add_child("subDevices", sub_devices);
  return tree;
}

pt::ptree device_properties_to_json(const ze_device_properties_t p) {
  pt::ptree tree;
  tree.put("name", p.name);
  tree.put("type", p.type);
  tree.put("vendorId", p.vendorId);
  tree.put("deviceId", p.deviceId);
  tree.put("uuid", to_string(p.uuid));
  tree.put("isSubdevice", bool(p.isSubdevice));
  tree.put("subdeviceId", p.subdeviceId);
  tree.put("coreClockRate", p.coreClockRate);
  tree.put("unifiedMemorySupported", bool(p.unifiedMemorySupported));
  tree.put("eccMemorySupported", bool(p.eccMemorySupported));
  tree.put("onDemandPageFaultsSupported", bool(p.onDemandPageFaultsSupported));
  tree.put("maxCommandQueues", p.maxCommandQueues);
  tree.put("numAsyncComputeEngines", p.numAsyncComputeEngines);
  tree.put("numAsyncCopyEngines", p.numAsyncCopyEngines);
  tree.put("numThreadsPerEU", p.numThreadsPerEU);
  tree.put("physicalEUSimdWidth", p.physicalEUSimdWidth);
  tree.put("numEUsPerSubslice", p.numEUsPerSubslice);
  tree.put("numSubslicesPerSlice", p.numSubslicesPerSlice);
  tree.put("numSlices", p.numSlices);
  tree.put("timerResolution", p.timerResolution);
  return tree;
}

pt::ptree
device_compute_properties_to_json(const ze_device_compute_properties_t p) {
  pt::ptree tree;
  tree.put("maxTotalGroupSize", p.maxTotalGroupSize);
  tree.put("maxGroupSizeX", p.maxGroupSizeX);
  tree.put("maxGroupSizeY", p.maxGroupSizeY);
  tree.put("maxGroupSizeZ", p.maxGroupSizeZ);
  tree.put("maxGroupCountX", p.maxGroupCountX);
  tree.put("maxGroupCountY", p.maxGroupCountY);
  tree.put("maxGroupCountZ", p.maxGroupCountZ);
  tree.put("maxSharedLocalMemory", p.maxSharedLocalMemory);
  tree.put("numSubGroupSizes", p.numSubGroupSizes);
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

pt::ptree
device_kernel_properties_to_json(const ze_device_kernel_properties_t p) {
  pt::ptree tree;
  tree.put("spirvVersionSupported", bool(p.spirvVersionSupported));
  tree.put("nativeKernelSupported", to_string(p.nativeKernelSupported));
  tree.put("fp16Supported", bool(p.fp16Supported));
  tree.put("fp64Supported", bool(p.fp64Supported));
  tree.put("int64AtomicsSupported", bool(p.int64AtomicsSupported));
  tree.put("dp4aSupported", bool(p.dp4aSupported));
  tree.put("halfFpCapabilities", to_string(p.halfFpCapabilities));
  tree.put("singleFpCapabilities", to_string(p.singleFpCapabilities));
  tree.put("doubleFpCapabilities", to_string(p.doubleFpCapabilities));
  tree.put("maxArgumentsSize", p.maxArgumentsSize);
  tree.put("printfBufferSize", p.printfBufferSize);
  return tree;
}

pt::ptree all_device_memory_properties_to_json(
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

pt::ptree
device_memory_properties_to_json(const ze_device_memory_properties_t p) {
  pt::ptree tree;
  tree.put("maxClockRate", p.maxClockRate);
  tree.put("maxBusWidth", p.maxBusWidth);
  tree.put("totalSize", p.totalSize);
  return tree;
}

pt::ptree
add_memory_access_properties(const ze_memory_access_capabilities_t c) {
  pt::ptree tree;
  if (c == ZE_MEMORY_ACCESS_NONE) {
    pt::ptree node;
    node.put("", to_string(ZE_MEMORY_ACCESS_NONE));
    tree.push_back(std::make_pair("", node));
  } else {
    if ((c & ZE_MEMORY_ACCESS) != 0) {
      pt::ptree node;
      node.put("", to_string(ZE_MEMORY_ACCESS));
      tree.push_back(std::make_pair("", node));
    }
    if ((c & ZE_MEMORY_ATOMIC_ACCESS) != 0) {
      pt::ptree node;
      node.put("", to_string(ZE_MEMORY_ATOMIC_ACCESS));
      tree.push_back(std::make_pair("", node));
    }
    if ((c & ZE_MEMORY_CONCURRENT_ACCESS) != 0) {
      pt::ptree node;
      node.put("", to_string(ZE_MEMORY_CONCURRENT_ACCESS));
      tree.push_back(std::make_pair("", node));
    }
    if ((c & ZE_MEMORY_CONCURRENT_ATOMIC_ACCESS) != 0) {
      pt::ptree node;
      node.put("", to_string(ZE_MEMORY_CONCURRENT_ATOMIC_ACCESS));
      tree.push_back(std::make_pair("", node));
    }
  }
  return tree;
}

pt::ptree device_memory_access_properties_to_json(
    const ze_device_memory_access_properties_t p) {
  pt::ptree tree;
  tree.add_child("hostAllocCapabilities",
                 add_memory_access_properties(p.hostAllocCapabilities));
  tree.add_child("deviceAllocCapabilities",
                 add_memory_access_properties(p.deviceAllocCapabilities));
  tree.add_child(
      "sharedSingleDeviceAllocCapabilities",
      add_memory_access_properties(p.sharedSingleDeviceAllocCapabilities));
  tree.add_child(
      "sharedCrossDeviceAllocCapabilities",
      add_memory_access_properties(p.sharedCrossDeviceAllocCapabilities));
  tree.add_child("sharedSystemAllocCapabilities",
                 add_memory_access_properties(p.sharedSystemAllocCapabilities));
  return tree;
}

pt::ptree
device_cache_properties_to_json(const ze_device_cache_properties_t p) {
  pt::ptree tree;
  tree.put("intermediateCacheControlSupported",
           bool(p.intermediateCacheControlSupported));
  tree.put("intermediateCacheSize", p.intermediateCacheSize);
  tree.put("intermediateCachelineSize", p.intermediateCachelineSize);
  tree.put("lastLevelCacheSizeControlSupported",
           bool(p.lastLevelCacheSizeControlSupported));
  tree.put("lastLevelCacheSize", p.lastLevelCacheSize);
  tree.put("lastLevelCachelineSize", p.lastLevelCachelineSize);
  return tree;
}

pt::ptree
device_image_properties_to_json(const ze_device_image_properties_t p) {
  pt::ptree tree;
  tree.put("supported", bool(p.supported));
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
  reg = std::regex(R"(\"([0-9]+\.{0,1}[0-9]*)\")");
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
