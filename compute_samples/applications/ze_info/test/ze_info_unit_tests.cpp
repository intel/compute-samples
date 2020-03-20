/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gmock/gmock.h"
#include "ze_info/text_formatter.hpp"
#include "ze_info/json_formatter.hpp"

#include <sstream>

#include <boost/property_tree/ptree.hpp>

namespace cs = compute_samples;
namespace pt = boost::property_tree;

class TextFormatterTests : public ::testing::Test {
protected:
  const int indentation_level_ = 1;
};

TEST_F(TextFormatterTests, DriversCapabilitiesToText) {
  const std::vector<cs::DriverCapabilities> capabilities = {
      cs::DriverCapabilities{}, cs::DriverCapabilities{}};
  const std::string actual =
      cs::drivers_capabilities_to_text(capabilities, indentation_level_);

  std::stringstream ss;
  ss << cs::key_value_to_text("Number of drivers", "2", indentation_level_);
  ss << cs::driver_capabilities_to_text(capabilities[0],
                                        indentation_level_ + 1);
  ss << cs::driver_capabilities_to_text(capabilities[1],
                                        indentation_level_ + 1);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DriverCapabilitiesToText) {
  const cs::DriverCapabilities capabilities = {
      ZE_API_VERSION_1_0,
      {ZE_DRIVER_PROPERTIES_VERSION_CURRENT,
       {0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12,
        0x78, 0x56, 0x34, 0x12},
       123},
      {ZE_DRIVER_IPC_PROPERTIES_VERSION_CURRENT, 1, 0},
      {cs::DeviceCapabilities{}, cs::DeviceCapabilities{}}};
  const std::string actual =
      cs::driver_capabilities_to_text(capabilities, indentation_level_);

  std::stringstream ss;
  ss << cs::driver_api_version_to_text(capabilities.api_version,
                                       indentation_level_);
  ss << cs::driver_properties_to_text(capabilities.driver_properties,
                                      indentation_level_);
  ss << cs::driver_ipc_properties_to_text(capabilities.ipc_properties,
                                          indentation_level_);
  ss << cs::key_value_to_text("Number of devices", "2", indentation_level_);
  ss << cs::device_capabilities_to_text(capabilities.devices[0],
                                        indentation_level_ + 1);
  ss << cs::device_capabilities_to_text(capabilities.devices[1],
                                        indentation_level_ + 1);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DriverPropertiesToText) {
  const ze_driver_properties_t p = {ZE_DRIVER_PROPERTIES_VERSION_CURRENT,
                                    {0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34,
                                     0x12, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56,
                                     0x34, 0x12},
                                    1234};
  const std::string actual =
      cs::driver_properties_to_text(p, indentation_level_);

  std::stringstream ss;
  ss << cs::key_value_to_text("Driver version", "1234", indentation_level_);
  ss << cs::key_value_to_text("UUID", "12345678-1234-5678-1234-567812345678",
                              indentation_level_);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DriverApiVersionToText) {
  const ze_api_version_t v = ZE_API_VERSION_1_0;
  const std::string actual =
      cs::driver_api_version_to_text(v, indentation_level_);

  std::stringstream ss;
  ss << cs::key_value_to_text("Driver API version", "0.91", indentation_level_);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DriverIpcPropertiesToText) {
  const ze_driver_ipc_properties_t p = {
      ZE_DRIVER_IPC_PROPERTIES_VERSION_CURRENT, 1, 0};
  const std::string actual =
      cs::driver_ipc_properties_to_text(p, indentation_level_);

  std::stringstream ss;
  ss << cs::key_value_to_text("IPC memory supported", "true",
                              indentation_level_);
  ss << cs::key_value_to_text("IPC events supported", "false",
                              indentation_level_);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceCapabilitiesToText) {
  const cs::DeviceCapabilities capabilities = {
      {}, {}, {}, {},
      {}, {}, {}, {cs::DeviceCapabilities{}, cs::DeviceCapabilities{}}};
  const std::string actual =
      cs::device_capabilities_to_text(capabilities, indentation_level_);

  std::stringstream ss;
  ss << cs::device_properties_to_text(capabilities.device_properties,
                                      indentation_level_);
  ss << cs::device_compute_properties_to_text(capabilities.compute_properties,
                                              indentation_level_);
  ss << cs::device_kernel_properties_to_text(capabilities.kernel_properties,
                                             indentation_level_);
  ss << cs::all_device_memory_properties_to_text(capabilities.memory_properties,
                                                 indentation_level_);
  ss << cs::device_memory_access_properties_to_text(
      capabilities.memory_access_properties, indentation_level_);
  ss << cs::device_cache_properties_to_text(capabilities.cache_properties,
                                            indentation_level_);
  ss << cs::device_image_properties_to_text(capabilities.image_properties,
                                            indentation_level_);
  ss << cs::key_value_to_text("Number of sub-devices", "2", indentation_level_);
  ss << cs::device_capabilities_to_text(capabilities.sub_devices[0],
                                        indentation_level_ + 1);
  ss << cs::device_capabilities_to_text(capabilities.sub_devices[1],
                                        indentation_level_ + 1);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DevicePropertiesToText) {
  const ze_device_properties_t p = {ZE_DEVICE_PROPERTIES_VERSION_CURRENT,
                                    ZE_DEVICE_TYPE_GPU,
                                    1,
                                    2,
                                    {0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34,
                                     0x12, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56,
                                     0x34, 0x12},
                                    1,
                                    3,
                                    4,
                                    1,
                                    1,
                                    1,
                                    5,
                                    6,
                                    7,
                                    8,
                                    9,
                                    10,
                                    11,
                                    12,
                                    13,
                                    14,
                                    "device_name"};
  const std::string actual =
      cs::device_properties_to_text(p, indentation_level_);

  std::stringstream ss;
  ss << cs::key_value_to_text("Device name", "device_name", indentation_level_);
  ss << cs::key_value_to_text("Device type", "ZE_DEVICE_TYPE_GPU",
                              indentation_level_);
  ss << cs::key_value_to_text("Vendor ID", "1", indentation_level_);
  ss << cs::key_value_to_text("Device ID", "2", indentation_level_);
  ss << cs::key_value_to_text("UUID", "12345678-1234-5678-1234-567812345678",
                              indentation_level_);
  ss << cs::key_value_to_text("Is sub-device", "true", indentation_level_);
  ss << cs::key_value_to_text("Sub-device ID", "3", indentation_level_);
  ss << cs::key_value_to_text("Core clock rate", "4", indentation_level_);
  ss << cs::key_value_to_text("Is unified memory supported", "true",
                              indentation_level_);
  ss << cs::key_value_to_text("Is ECC memory supported", "true",
                              indentation_level_);
  ss << cs::key_value_to_text("Are on demand page faults supported", "true",
                              indentation_level_);
  ss << cs::key_value_to_text("Max command queues", "5", indentation_level_);
  ss << cs::key_value_to_text("Number of asynchronous compute engines", "6",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of asynchronous copy engines", "7",
                              indentation_level_);
  ss << cs::key_value_to_text("Max command queue priority", "8",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of threads per EU", "9",
                              indentation_level_);
  ss << cs::key_value_to_text("Physical EU SIMD width", "10",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of EUs per subslice", "11",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of subslices per slice", "12",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of slices", "13", indentation_level_);
  ss << cs::key_value_to_text("Timer resolution", "14", indentation_level_);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceComputePropertiesToText) {
  const ze_device_compute_properties_t p = {
      ZE_DEVICE_COMPUTE_PROPERTIES_VERSION_CURRENT,
      1,
      2,
      3,
      4,
      5,
      6,
      7,
      8,
      2,
      {8, 9, 0, 0, 0, 0, 0, 0}};
  const std::string actual =
      cs::device_compute_properties_to_text(p, indentation_level_);

  std::stringstream ss;
  ss << cs::key_value_to_text("Max total group size", "1", indentation_level_);
  ss << cs::key_value_to_text("Max group size X", "2", indentation_level_);
  ss << cs::key_value_to_text("Max group size Y", "3", indentation_level_);
  ss << cs::key_value_to_text("Max group size Z", "4", indentation_level_);
  ss << cs::key_value_to_text("Max group count X", "5", indentation_level_);
  ss << cs::key_value_to_text("Max group count Y", "6", indentation_level_);
  ss << cs::key_value_to_text("Max group count Z", "7", indentation_level_);
  ss << cs::key_value_to_text("Max shared local memory", "8",
                              indentation_level_);
  ss << cs::key_value_to_text("Sub-group sizes", "8 9", indentation_level_);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceKernelPropertiesToText) {
  const ze_device_kernel_properties_t p = {
      ZE_DEVICE_KERNEL_PROPERTIES_VERSION_CURRENT,
      1,
      {0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12,
       0x78, 0x56, 0x34, 0x12},
      1,
      0,
      1,
      1,
      static_cast<ze_fp_capabilities_t>(ZE_FP_CAPS_NONE),
      static_cast<ze_fp_capabilities_t>(ZE_FP_CAPS_NONE),
      static_cast<ze_fp_capabilities_t>(ZE_FP_CAPS_DENORM |
                                        ZE_FP_CAPS_ROUNDED_DIVIDE_SQRT),
      2,
      3};
  const std::string actual =
      cs::device_kernel_properties_to_text(p, indentation_level_);

  std::stringstream ss;
  ss << cs::key_value_to_text("SPIR-V version", "1", indentation_level_);
  ss << cs::key_value_to_text("Native kernel supported",
                              "12345678-1234-5678-1234-567812345678",
                              indentation_level_);
  ss << cs::key_value_to_text("FP16 supported", "true", indentation_level_);
  ss << cs::key_value_to_text("FP64 supported", "false", indentation_level_);
  ss << cs::key_value_to_text("INT64 atomics supported", "true",
                              indentation_level_);
  ss << cs::key_value_to_text("DP4A supported", "true", indentation_level_);
  ss << cs::key_value_to_text("Half-precision floating-point capabilities",
                              "ZE_FP_CAPS_NONE", indentation_level_);
  ss << cs::key_value_to_text("Single-precision floating-point capabilities",
                              "ZE_FP_CAPS_NONE", indentation_level_);
  ss << cs::key_value_to_text(
      "Double-precision floating-point capabilities",
      "ZE_FP_CAPS_DENORM | ZE_FP_CAPS_ROUNDED_DIVIDE_SQRT", indentation_level_);
  ss << cs::key_value_to_text("Max argument size", "2", indentation_level_);
  ss << cs::key_value_to_text("printf buffer size", "3", indentation_level_);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, AllDeviceMemoryPropertiesToText) {
  const std::vector<ze_device_memory_properties_t> p = {
      {ZE_DEVICE_MEMORY_PROPERTIES_VERSION_CURRENT, 1, 2, 3},
      {ZE_DEVICE_MEMORY_PROPERTIES_VERSION_CURRENT, 4, 5, 6}};
  const std::string actual =
      cs::all_device_memory_properties_to_text(p, indentation_level_);

  std::stringstream ss;
  ss << cs::key_value_to_text("Number of memory properties", "2",
                              indentation_level_);
  ss << cs::device_memory_properties_to_text(p[0], indentation_level_ + 1);
  ss << cs::device_memory_properties_to_text(p[1], indentation_level_ + 1);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceMemoryPropertiesToText) {
  const ze_device_memory_properties_t p = {
      ZE_DEVICE_MEMORY_PROPERTIES_VERSION_CURRENT, 1, 2, 3};
  const std::string actual =
      cs::device_memory_properties_to_text(p, indentation_level_);

  std::stringstream ss;
  ss << cs::key_value_to_text("Max clock rate", "1", indentation_level_);
  ss << cs::key_value_to_text("Max bus width", "2", indentation_level_);
  ss << cs::key_value_to_text("Total size", "3", indentation_level_);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceMemoryAccessPropertiesToText) {
  const ze_device_memory_access_properties_t p = {
      ZE_DEVICE_MEMORY_ACCESS_PROPERTIES_VERSION_CURRENT,
      static_cast<ze_memory_access_capabilities_t>(ZE_MEMORY_ACCESS_NONE),
      static_cast<ze_memory_access_capabilities_t>(ZE_MEMORY_ACCESS),
      static_cast<ze_memory_access_capabilities_t>(ZE_MEMORY_ATOMIC_ACCESS),
      static_cast<ze_memory_access_capabilities_t>(ZE_MEMORY_CONCURRENT_ACCESS),
      static_cast<ze_memory_access_capabilities_t>(ZE_MEMORY_ACCESS |
                                                   ZE_MEMORY_ATOMIC_ACCESS)};
  const std::string actual =
      cs::device_memory_access_properties_to_text(p, indentation_level_);

  std::stringstream ss;
  ss << cs::key_value_to_text("Host allocation capabilities",
                              "ZE_MEMORY_ACCESS_NONE", indentation_level_);
  ss << cs::key_value_to_text("Device allocation capabilities",
                              "ZE_MEMORY_ACCESS", indentation_level_);
  ss << cs::key_value_to_text("Shared single-device allocation capabilities",
                              "ZE_MEMORY_ATOMIC_ACCESS", indentation_level_);
  ss << cs::key_value_to_text("Shared cross-device allocation capabilities",
                              "ZE_MEMORY_CONCURRENT_ACCESS",
                              indentation_level_);
  ss << cs::key_value_to_text("Shared system allocation capabilities",
                              "ZE_MEMORY_ACCESS | ZE_MEMORY_ATOMIC_ACCESS",
                              indentation_level_);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceCachePropertiesToText) {
  const ze_device_cache_properties_t p = {
      ZE_DEVICE_CACHE_PROPERTIES_VERSION_CURRENT, 1, 2, 3, 1, 4, 5};
  const std::string actual =
      cs::device_cache_properties_to_text(p, indentation_level_);

  std::stringstream ss;
  ss << cs::key_value_to_text("Intermediate cache control supported", "true",
                              indentation_level_);
  ss << cs::key_value_to_text("Intermediate cache size", "2",
                              indentation_level_);
  ss << cs::key_value_to_text("Intermediate cacheline size", "3",
                              indentation_level_);
  ss << cs::key_value_to_text("Last level cache size control supported", "true",
                              indentation_level_);
  ss << cs::key_value_to_text("Last level cache size", "4", indentation_level_);
  ss << cs::key_value_to_text("Last level cacheline size", "5",
                              indentation_level_);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceImagePropertiesToText) {
  const ze_device_image_properties_t p = {
      ZE_DEVICE_IMAGE_PROPERTIES_VERSION_CURRENT, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  const std::string actual =
      cs::device_image_properties_to_text(p, indentation_level_);

  std::stringstream ss;
  ss << cs::key_value_to_text("Are images supported", "true",
                              indentation_level_);
  ss << cs::key_value_to_text("Max image 1D", "2", indentation_level_);
  ss << cs::key_value_to_text("Max image 2D", "3", indentation_level_);
  ss << cs::key_value_to_text("Max image 3D", "4", indentation_level_);
  ss << cs::key_value_to_text("Max image buffer slices", "5",
                              indentation_level_);
  ss << cs::key_value_to_text("Max image array slices", "6",
                              indentation_level_);
  ss << cs::key_value_to_text("Max number of samplers", "7",
                              indentation_level_);
  ss << cs::key_value_to_text("Max number of read arguments", "8",
                              indentation_level_);
  ss << cs::key_value_to_text("Max number of write arguments", "9",
                              indentation_level_);
  const std::string expected = ss.str();

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, KeyValueToText) {
  const std::string actual =
      cs::key_value_to_text("Key", "Value", indentation_level_);

  const std::string expected = "    Key                                        "
                               "                                 Value\n";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DriversCapabilitiesToJSON) {
  const std::vector<cs::DriverCapabilities> capabilities = {
      cs::DriverCapabilities{}, cs::DriverCapabilities{}};

  const pt::ptree json = cs::drivers_capabilities_to_json(capabilities);

  const std::string actual = cs::ptree_to_string(json);
  const std::string expected =
      "[\n"
      "        {\n"
      "            \"ze_api_version_t\": 0.0,\n"
      "            \"ze_driver_properties_t\": {\n"
      "                \"uuid\": \"00000-00-00-00-000000\",\n"
      "                \"driverVersion\": 0\n"
      "            },\n"
      "            \"ze_driver_ipc_properties_t\": {\n"
      "                \"memsSupported\": false,\n"
      "                \"eventsSupported\": false\n"
      "            },\n"
      "            \"devices\": []\n"
      "        },\n"
      "        {\n"
      "            \"ze_api_version_t\": 0.0,\n"
      "            \"ze_driver_properties_t\": {\n"
      "                \"uuid\": \"00000-00-00-00-000000\",\n"
      "                \"driverVersion\": 0\n"
      "            },\n"
      "            \"ze_driver_ipc_properties_t\": {\n"
      "                \"memsSupported\": false,\n"
      "                \"eventsSupported\": false\n"
      "            },\n"
      "            \"devices\": []\n"
      "        }\n"
      "]";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DriverCapabilitiesToJSON) {
  const cs::DriverCapabilities capabilities = {
      ZE_API_VERSION_1_0,
      {ZE_DRIVER_PROPERTIES_VERSION_CURRENT,
       {0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12,
        0x78, 0x56, 0x34, 0x12},
       123},
      {ZE_DRIVER_IPC_PROPERTIES_VERSION_CURRENT, 1, 0},
      {cs::DeviceCapabilities{}, cs::DeviceCapabilities{}}};

  const pt::ptree json = cs::driver_capabilities_to_json(capabilities);

  const std::string actual = cs::ptree_to_string(json);
  const std::string expected =
      "{\n"
      "    \"ze_api_version_t\": 0.91,\n"
      "    \"ze_driver_properties_t\": {\n"
      "        \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "        \"driverVersion\": 123\n"
      "    },\n"
      "    \"ze_driver_ipc_properties_t\": {\n"
      "        \"memsSupported\": true,\n"
      "        \"eventsSupported\": false\n"
      "    },\n"
      "    \"devices\": [\n"
      "        {\n"
      "            \"ze_device_properties_t\": {\n"
      "                \"name\": \"\",\n"
      "                \"type\": \"Unknown ze_device_type_t value: 0\",\n"
      "                \"vendorId\": 0,\n"
      "                \"deviceId\": 0,\n"
      "                \"uuid\": \"00000-00-00-00-000000\",\n"
      "                \"isSubdevice\": false,\n"
      "                \"subdeviceId\": 0,\n"
      "                \"coreClockRate\": 0,\n"
      "                \"unifiedMemorySupported\": false,\n"
      "                \"eccMemorySupported\": false,\n"
      "                \"onDemandPageFaultsSupported\": false,\n"
      "                \"maxCommandQueues\": 0,\n"
      "                \"numAsyncComputeEngines\": 0,\n"
      "                \"numAsyncCopyEngines\": 0,\n"
      "                \"numThreadsPerEU\": 0,\n"
      "                \"physicalEUSimdWidth\": 0,\n"
      "                \"numEUsPerSubslice\": 0,\n"
      "                \"numSubslicesPerSlice\": 0,\n"
      "                \"numSlices\": 0,\n"
      "                \"timerResolution\": 0\n"
      "            },\n"
      "            \"ze_device_compute_properties_t\": {\n"
      "                \"maxTotalGroupSize\": 0,\n"
      "                \"maxGroupSizeX\": 0,\n"
      "                \"maxGroupSizeY\": 0,\n"
      "                \"maxGroupSizeZ\": 0,\n"
      "                \"maxGroupCountX\": 0,\n"
      "                \"maxGroupCountY\": 0,\n"
      "                \"maxGroupCountZ\": 0,\n"
      "                \"maxSharedLocalMemory\": 0,\n"
      "                \"numSubGroupSizes\": 0,\n"
      "                \"subGroupSizes\": []\n"
      "            },\n"
      "            \"ze_device_kernel_properties_t\": {\n"
      "                \"spirvVersionSupported\": false,\n"
      "                \"nativeKernelSupported\": \"00000-00-00-00-000000\",\n"
      "                \"fp16Supported\": false,\n"
      "                \"fp64Supported\": false,\n"
      "                \"int64AtomicsSupported\": false,\n"
      "                \"dp4aSupported\": false,\n"
      "                \"halfFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "                \"singleFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "                \"doubleFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "                \"maxArgumentsSize\": 0,\n"
      "                \"printfBufferSize\": 0\n"
      "            },\n"
      "            \"ze_device_memory_properties_t\": [],\n"
      "            \"ze_device_memory_access_properties_t\": {\n"
      "                \"hostAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"deviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"sharedSingleDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"sharedCrossDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"sharedSystemAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_cache_properties_t\": {\n"
      "                \"intermediateCacheControlSupported\": false,\n"
      "                \"intermediateCacheSize\": 0,\n"
      "                \"intermediateCachelineSize\": 0,\n"
      "                \"lastLevelCacheSizeControlSupported\": false,\n"
      "                \"lastLevelCacheSize\": 0,\n"
      "                \"lastLevelCachelineSize\": 0\n"
      "            },\n"
      "            \"ze_device_image_properties_t\": {\n"
      "                \"supported\": false,\n"
      "                \"maxImageDims1D\": 0,\n"
      "                \"maxImageDims2D\": 0,\n"
      "                \"maxImageDims3D\": 0,\n"
      "                \"maxImageBufferSize\": 0,\n"
      "                \"maxImageArraySlices\": 0,\n"
      "                \"maxSamplers\": 0,\n"
      "                \"maxReadImageArgs\": 0,\n"
      "                \"maxWriteImageArgs\": 0\n"
      "            },\n"
      "            \"subDevices\": []\n"
      "        },\n"
      "        {\n"
      "            \"ze_device_properties_t\": {\n"
      "                \"name\": \"\",\n"
      "                \"type\": \"Unknown ze_device_type_t value: 0\",\n"
      "                \"vendorId\": 0,\n"
      "                \"deviceId\": 0,\n"
      "                \"uuid\": \"00000-00-00-00-000000\",\n"
      "                \"isSubdevice\": false,\n"
      "                \"subdeviceId\": 0,\n"
      "                \"coreClockRate\": 0,\n"
      "                \"unifiedMemorySupported\": false,\n"
      "                \"eccMemorySupported\": false,\n"
      "                \"onDemandPageFaultsSupported\": false,\n"
      "                \"maxCommandQueues\": 0,\n"
      "                \"numAsyncComputeEngines\": 0,\n"
      "                \"numAsyncCopyEngines\": 0,\n"
      "                \"numThreadsPerEU\": 0,\n"
      "                \"physicalEUSimdWidth\": 0,\n"
      "                \"numEUsPerSubslice\": 0,\n"
      "                \"numSubslicesPerSlice\": 0,\n"
      "                \"numSlices\": 0,\n"
      "                \"timerResolution\": 0\n"
      "            },\n"
      "            \"ze_device_compute_properties_t\": {\n"
      "                \"maxTotalGroupSize\": 0,\n"
      "                \"maxGroupSizeX\": 0,\n"
      "                \"maxGroupSizeY\": 0,\n"
      "                \"maxGroupSizeZ\": 0,\n"
      "                \"maxGroupCountX\": 0,\n"
      "                \"maxGroupCountY\": 0,\n"
      "                \"maxGroupCountZ\": 0,\n"
      "                \"maxSharedLocalMemory\": 0,\n"
      "                \"numSubGroupSizes\": 0,\n"
      "                \"subGroupSizes\": []\n"
      "            },\n"
      "            \"ze_device_kernel_properties_t\": {\n"
      "                \"spirvVersionSupported\": false,\n"
      "                \"nativeKernelSupported\": \"00000-00-00-00-000000\",\n"
      "                \"fp16Supported\": false,\n"
      "                \"fp64Supported\": false,\n"
      "                \"int64AtomicsSupported\": false,\n"
      "                \"dp4aSupported\": false,\n"
      "                \"halfFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "                \"singleFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "                \"doubleFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "                \"maxArgumentsSize\": 0,\n"
      "                \"printfBufferSize\": 0\n"
      "            },\n"
      "            \"ze_device_memory_properties_t\": [],\n"
      "            \"ze_device_memory_access_properties_t\": {\n"
      "                \"hostAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"deviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"sharedSingleDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"sharedCrossDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"sharedSystemAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_cache_properties_t\": {\n"
      "                \"intermediateCacheControlSupported\": false,\n"
      "                \"intermediateCacheSize\": 0,\n"
      "                \"intermediateCachelineSize\": 0,\n"
      "                \"lastLevelCacheSizeControlSupported\": false,\n"
      "                \"lastLevelCacheSize\": 0,\n"
      "                \"lastLevelCachelineSize\": 0\n"
      "            },\n"
      "            \"ze_device_image_properties_t\": {\n"
      "                \"supported\": false,\n"
      "                \"maxImageDims1D\": 0,\n"
      "                \"maxImageDims2D\": 0,\n"
      "                \"maxImageDims3D\": 0,\n"
      "                \"maxImageBufferSize\": 0,\n"
      "                \"maxImageArraySlices\": 0,\n"
      "                \"maxSamplers\": 0,\n"
      "                \"maxReadImageArgs\": 0,\n"
      "                \"maxWriteImageArgs\": 0\n"
      "            },\n"
      "            \"subDevices\": []\n"
      "        }\n"
      "    ]\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DriverIpcPropertiesToJSON) {
  const ze_driver_ipc_properties_t p = {
      ZE_DRIVER_IPC_PROPERTIES_VERSION_CURRENT, 1, 0};

  const pt::ptree json = cs::driver_ipc_properties_to_json(p);

  const std::string actual = cs::ptree_to_string(json);
  const std::string expected = "{\n"
                               "    \"memsSupported\": true,\n"
                               "    \"eventsSupported\": false\n"
                               "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceCapabilitiesToJSON) {
  const cs::DeviceCapabilities capabilities = {
      {}, {}, {}, {},
      {}, {}, {}, {cs::DeviceCapabilities{}, cs::DeviceCapabilities{}}};

  const pt::ptree json = cs::device_capabilities_to_json(capabilities);

  const std::string actual = cs::ptree_to_string(json);
  const std::string expected =
      "{\n"
      "    \"ze_device_properties_t\": {\n"
      "        \"name\": \"\",\n"
      "        \"type\": \"Unknown ze_device_type_t value: 0\",\n"
      "        \"vendorId\": 0,\n"
      "        \"deviceId\": 0,\n"
      "        \"uuid\": \"00000-00-00-00-000000\",\n"
      "        \"isSubdevice\": false,\n"
      "        \"subdeviceId\": 0,\n"
      "        \"coreClockRate\": 0,\n"
      "        \"unifiedMemorySupported\": false,\n"
      "        \"eccMemorySupported\": false,\n"
      "        \"onDemandPageFaultsSupported\": false,\n"
      "        \"maxCommandQueues\": 0,\n"
      "        \"numAsyncComputeEngines\": 0,\n"
      "        \"numAsyncCopyEngines\": 0,\n"
      "        \"numThreadsPerEU\": 0,\n"
      "        \"physicalEUSimdWidth\": 0,\n"
      "        \"numEUsPerSubslice\": 0,\n"
      "        \"numSubslicesPerSlice\": 0,\n"
      "        \"numSlices\": 0,\n"
      "        \"timerResolution\": 0\n"
      "    },\n"
      "    \"ze_device_compute_properties_t\": {\n"
      "        \"maxTotalGroupSize\": 0,\n"
      "        \"maxGroupSizeX\": 0,\n"
      "        \"maxGroupSizeY\": 0,\n"
      "        \"maxGroupSizeZ\": 0,\n"
      "        \"maxGroupCountX\": 0,\n"
      "        \"maxGroupCountY\": 0,\n"
      "        \"maxGroupCountZ\": 0,\n"
      "        \"maxSharedLocalMemory\": 0,\n"
      "        \"numSubGroupSizes\": 0,\n"
      "        \"subGroupSizes\": []\n"
      "    },\n"
      "    \"ze_device_kernel_properties_t\": {\n"
      "        \"spirvVersionSupported\": false,\n"
      "        \"nativeKernelSupported\": \"00000-00-00-00-000000\",\n"
      "        \"fp16Supported\": false,\n"
      "        \"fp64Supported\": false,\n"
      "        \"int64AtomicsSupported\": false,\n"
      "        \"dp4aSupported\": false,\n"
      "        \"halfFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "        \"singleFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "        \"doubleFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "        \"maxArgumentsSize\": 0,\n"
      "        \"printfBufferSize\": 0\n"
      "    },\n"
      "    \"ze_device_memory_properties_t\": [],\n"
      "    \"ze_device_memory_access_properties_t\": {\n"
      "        \"hostAllocCapabilities\": [\n"
      "            \"ZE_MEMORY_ACCESS_NONE\"\n"
      "        ],\n"
      "        \"deviceAllocCapabilities\": [\n"
      "            \"ZE_MEMORY_ACCESS_NONE\"\n"
      "        ],\n"
      "        \"sharedSingleDeviceAllocCapabilities\": [\n"
      "            \"ZE_MEMORY_ACCESS_NONE\"\n"
      "        ],\n"
      "        \"sharedCrossDeviceAllocCapabilities\": [\n"
      "            \"ZE_MEMORY_ACCESS_NONE\"\n"
      "        ],\n"
      "        \"sharedSystemAllocCapabilities\": [\n"
      "            \"ZE_MEMORY_ACCESS_NONE\"\n"
      "        ]\n"
      "    },\n"
      "    \"ze_device_cache_properties_t\": {\n"
      "        \"intermediateCacheControlSupported\": false,\n"
      "        \"intermediateCacheSize\": 0,\n"
      "        \"intermediateCachelineSize\": 0,\n"
      "        \"lastLevelCacheSizeControlSupported\": false,\n"
      "        \"lastLevelCacheSize\": 0,\n"
      "        \"lastLevelCachelineSize\": 0\n"
      "    },\n"
      "    \"ze_device_image_properties_t\": {\n"
      "        \"supported\": false,\n"
      "        \"maxImageDims1D\": 0,\n"
      "        \"maxImageDims2D\": 0,\n"
      "        \"maxImageDims3D\": 0,\n"
      "        \"maxImageBufferSize\": 0,\n"
      "        \"maxImageArraySlices\": 0,\n"
      "        \"maxSamplers\": 0,\n"
      "        \"maxReadImageArgs\": 0,\n"
      "        \"maxWriteImageArgs\": 0\n"
      "    },\n"
      "    \"subDevices\": [\n"
      "        {\n"
      "            \"ze_device_properties_t\": {\n"
      "                \"name\": \"\",\n"
      "                \"type\": \"Unknown ze_device_type_t value: 0\",\n"
      "                \"vendorId\": 0,\n"
      "                \"deviceId\": 0,\n"
      "                \"uuid\": \"00000-00-00-00-000000\",\n"
      "                \"isSubdevice\": false,\n"
      "                \"subdeviceId\": 0,\n"
      "                \"coreClockRate\": 0,\n"
      "                \"unifiedMemorySupported\": false,\n"
      "                \"eccMemorySupported\": false,\n"
      "                \"onDemandPageFaultsSupported\": false,\n"
      "                \"maxCommandQueues\": 0,\n"
      "                \"numAsyncComputeEngines\": 0,\n"
      "                \"numAsyncCopyEngines\": 0,\n"
      "                \"numThreadsPerEU\": 0,\n"
      "                \"physicalEUSimdWidth\": 0,\n"
      "                \"numEUsPerSubslice\": 0,\n"
      "                \"numSubslicesPerSlice\": 0,\n"
      "                \"numSlices\": 0,\n"
      "                \"timerResolution\": 0\n"
      "            },\n"
      "            \"ze_device_compute_properties_t\": {\n"
      "                \"maxTotalGroupSize\": 0,\n"
      "                \"maxGroupSizeX\": 0,\n"
      "                \"maxGroupSizeY\": 0,\n"
      "                \"maxGroupSizeZ\": 0,\n"
      "                \"maxGroupCountX\": 0,\n"
      "                \"maxGroupCountY\": 0,\n"
      "                \"maxGroupCountZ\": 0,\n"
      "                \"maxSharedLocalMemory\": 0,\n"
      "                \"numSubGroupSizes\": 0,\n"
      "                \"subGroupSizes\": []\n"
      "            },\n"
      "            \"ze_device_kernel_properties_t\": {\n"
      "                \"spirvVersionSupported\": false,\n"
      "                \"nativeKernelSupported\": \"00000-00-00-00-000000\",\n"
      "                \"fp16Supported\": false,\n"
      "                \"fp64Supported\": false,\n"
      "                \"int64AtomicsSupported\": false,\n"
      "                \"dp4aSupported\": false,\n"
      "                \"halfFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "                \"singleFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "                \"doubleFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "                \"maxArgumentsSize\": 0,\n"
      "                \"printfBufferSize\": 0\n"
      "            },\n"
      "            \"ze_device_memory_properties_t\": [],\n"
      "            \"ze_device_memory_access_properties_t\": {\n"
      "                \"hostAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"deviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"sharedSingleDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"sharedCrossDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"sharedSystemAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_cache_properties_t\": {\n"
      "                \"intermediateCacheControlSupported\": false,\n"
      "                \"intermediateCacheSize\": 0,\n"
      "                \"intermediateCachelineSize\": 0,\n"
      "                \"lastLevelCacheSizeControlSupported\": false,\n"
      "                \"lastLevelCacheSize\": 0,\n"
      "                \"lastLevelCachelineSize\": 0\n"
      "            },\n"
      "            \"ze_device_image_properties_t\": {\n"
      "                \"supported\": false,\n"
      "                \"maxImageDims1D\": 0,\n"
      "                \"maxImageDims2D\": 0,\n"
      "                \"maxImageDims3D\": 0,\n"
      "                \"maxImageBufferSize\": 0,\n"
      "                \"maxImageArraySlices\": 0,\n"
      "                \"maxSamplers\": 0,\n"
      "                \"maxReadImageArgs\": 0,\n"
      "                \"maxWriteImageArgs\": 0\n"
      "            },\n"
      "            \"subDevices\": []\n"
      "        },\n"
      "        {\n"
      "            \"ze_device_properties_t\": {\n"
      "                \"name\": \"\",\n"
      "                \"type\": \"Unknown ze_device_type_t value: 0\",\n"
      "                \"vendorId\": 0,\n"
      "                \"deviceId\": 0,\n"
      "                \"uuid\": \"00000-00-00-00-000000\",\n"
      "                \"isSubdevice\": false,\n"
      "                \"subdeviceId\": 0,\n"
      "                \"coreClockRate\": 0,\n"
      "                \"unifiedMemorySupported\": false,\n"
      "                \"eccMemorySupported\": false,\n"
      "                \"onDemandPageFaultsSupported\": false,\n"
      "                \"maxCommandQueues\": 0,\n"
      "                \"numAsyncComputeEngines\": 0,\n"
      "                \"numAsyncCopyEngines\": 0,\n"
      "                \"numThreadsPerEU\": 0,\n"
      "                \"physicalEUSimdWidth\": 0,\n"
      "                \"numEUsPerSubslice\": 0,\n"
      "                \"numSubslicesPerSlice\": 0,\n"
      "                \"numSlices\": 0,\n"
      "                \"timerResolution\": 0\n"
      "            },\n"
      "            \"ze_device_compute_properties_t\": {\n"
      "                \"maxTotalGroupSize\": 0,\n"
      "                \"maxGroupSizeX\": 0,\n"
      "                \"maxGroupSizeY\": 0,\n"
      "                \"maxGroupSizeZ\": 0,\n"
      "                \"maxGroupCountX\": 0,\n"
      "                \"maxGroupCountY\": 0,\n"
      "                \"maxGroupCountZ\": 0,\n"
      "                \"maxSharedLocalMemory\": 0,\n"
      "                \"numSubGroupSizes\": 0,\n"
      "                \"subGroupSizes\": []\n"
      "            },\n"
      "            \"ze_device_kernel_properties_t\": {\n"
      "                \"spirvVersionSupported\": false,\n"
      "                \"nativeKernelSupported\": \"00000-00-00-00-000000\",\n"
      "                \"fp16Supported\": false,\n"
      "                \"fp64Supported\": false,\n"
      "                \"int64AtomicsSupported\": false,\n"
      "                \"dp4aSupported\": false,\n"
      "                \"halfFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "                \"singleFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "                \"doubleFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "                \"maxArgumentsSize\": 0,\n"
      "                \"printfBufferSize\": 0\n"
      "            },\n"
      "            \"ze_device_memory_properties_t\": [],\n"
      "            \"ze_device_memory_access_properties_t\": {\n"
      "                \"hostAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"deviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"sharedSingleDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"sharedCrossDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ],\n"
      "                \"sharedSystemAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_NONE\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_cache_properties_t\": {\n"
      "                \"intermediateCacheControlSupported\": false,\n"
      "                \"intermediateCacheSize\": 0,\n"
      "                \"intermediateCachelineSize\": 0,\n"
      "                \"lastLevelCacheSizeControlSupported\": false,\n"
      "                \"lastLevelCacheSize\": 0,\n"
      "                \"lastLevelCachelineSize\": 0\n"
      "            },\n"
      "            \"ze_device_image_properties_t\": {\n"
      "                \"supported\": false,\n"
      "                \"maxImageDims1D\": 0,\n"
      "                \"maxImageDims2D\": 0,\n"
      "                \"maxImageDims3D\": 0,\n"
      "                \"maxImageBufferSize\": 0,\n"
      "                \"maxImageArraySlices\": 0,\n"
      "                \"maxSamplers\": 0,\n"
      "                \"maxReadImageArgs\": 0,\n"
      "                \"maxWriteImageArgs\": 0\n"
      "            },\n"
      "            \"subDevices\": []\n"
      "        }\n"
      "    ]\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DevicePropertiesToJSON) {
  const ze_device_properties_t p = {ZE_DEVICE_PROPERTIES_VERSION_CURRENT,
                                    ZE_DEVICE_TYPE_GPU,
                                    1,
                                    2,
                                    {0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34,
                                     0x12, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56,
                                     0x34, 0x12},
                                    1,
                                    3,
                                    4,
                                    1,
                                    1,
                                    1,
                                    5,
                                    6,
                                    7,
                                    8,
                                    9,
                                    10,
                                    11,
                                    12,
                                    13,
                                    14,
                                    "device_name"};

  const pt::ptree json = cs::device_properties_to_json(p);

  const std::string actual = cs::ptree_to_string(json);
  const std::string expected =
      "{\n"
      "    \"name\": \"device_name\",\n"
      "    \"type\": \"ZE_DEVICE_TYPE_GPU\",\n"
      "    \"vendorId\": 1,\n"
      "    \"deviceId\": 2,\n"
      "    \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "    \"isSubdevice\": true,\n"
      "    \"subdeviceId\": 3,\n"
      "    \"coreClockRate\": 4,\n"
      "    \"unifiedMemorySupported\": true,\n"
      "    \"eccMemorySupported\": true,\n"
      "    \"onDemandPageFaultsSupported\": true,\n"
      "    \"maxCommandQueues\": 5,\n"
      "    \"numAsyncComputeEngines\": 6,\n"
      "    \"numAsyncCopyEngines\": 7,\n"
      "    \"numThreadsPerEU\": 9,\n"
      "    \"physicalEUSimdWidth\": 10,\n"
      "    \"numEUsPerSubslice\": 11,\n"
      "    \"numSubslicesPerSlice\": 12,\n"
      "    \"numSlices\": 13,\n"
      "    \"timerResolution\": 14\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceComputePropertiesToJSON) {
  const ze_device_compute_properties_t p = {
      ZE_DEVICE_COMPUTE_PROPERTIES_VERSION_CURRENT,
      1,
      2,
      3,
      4,
      5,
      6,
      7,
      8,
      2,
      {8, 9, 0, 0, 0, 0, 0, 0}};

  const pt::ptree json = cs::device_compute_properties_to_json(p);

  const std::string actual = cs::ptree_to_string(json);
  const std::string expected = "{\n"
                               "    \"maxTotalGroupSize\": 1,\n"
                               "    \"maxGroupSizeX\": 2,\n"
                               "    \"maxGroupSizeY\": 3,\n"
                               "    \"maxGroupSizeZ\": 4,\n"
                               "    \"maxGroupCountX\": 5,\n"
                               "    \"maxGroupCountY\": 6,\n"
                               "    \"maxGroupCountZ\": 7,\n"
                               "    \"maxSharedLocalMemory\": 8,\n"
                               "    \"numSubGroupSizes\": 2,\n"
                               "    \"subGroupSizes\": [\n"
                               "        8,\n"
                               "        9\n"
                               "    ]\n"
                               "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceKernelPropertiesToJSON) {
  const ze_device_kernel_properties_t p = {
      ZE_DEVICE_KERNEL_PROPERTIES_VERSION_CURRENT,
      1,
      {0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12,
       0x78, 0x56, 0x34, 0x12},
      1,
      0,
      1,
      1,
      static_cast<ze_fp_capabilities_t>(ZE_FP_CAPS_NONE),
      static_cast<ze_fp_capabilities_t>(ZE_FP_CAPS_NONE),
      static_cast<ze_fp_capabilities_t>(ZE_FP_CAPS_DENORM |
                                        ZE_FP_CAPS_ROUNDED_DIVIDE_SQRT),
      2,
      3};

  const pt::ptree json = cs::device_kernel_properties_to_json(p);

  const std::string actual = cs::ptree_to_string(json);
  const std::string expected =
      "{\n"
      "    \"spirvVersionSupported\": true,\n"
      "    \"nativeKernelSupported\": "
      "\"12345678-1234-5678-1234-567812345678\",\n"
      "    \"fp16Supported\": true,\n"
      "    \"fp64Supported\": false,\n"
      "    \"int64AtomicsSupported\": true,\n"
      "    \"dp4aSupported\": true,\n"
      "    \"halfFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "    \"singleFpCapabilities\": \"ZE_FP_CAPS_NONE\",\n"
      "    \"doubleFpCapabilities\": \"ZE_FP_CAPS_DENORM | "
      "ZE_FP_CAPS_ROUNDED_DIVIDE_SQRT\",\n"
      "    \"maxArgumentsSize\": 2,\n"
      "    \"printfBufferSize\": 3\n"
      "}";
  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, AllDeviceMemoryPropertiesToJSON) {
  const std::vector<ze_device_memory_properties_t> p = {
      {ZE_DEVICE_MEMORY_PROPERTIES_VERSION_CURRENT, 1, 2, 3},
      {ZE_DEVICE_MEMORY_PROPERTIES_VERSION_CURRENT, 4, 5, 6}};

  const pt::ptree json = cs::all_device_memory_properties_to_json(p);

  const std::string actual = cs::ptree_to_string(json);
  const std::string expected = "[\n"
                               "        {\n"
                               "            \"maxClockRate\": 1,\n"
                               "            \"maxBusWidth\": 2,\n"
                               "            \"totalSize\": 3\n"
                               "        },\n"
                               "        {\n"
                               "            \"maxClockRate\": 4,\n"
                               "            \"maxBusWidth\": 5,\n"
                               "            \"totalSize\": 6\n"
                               "        }\n"
                               "]";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceMemoryPropertiesToJSON) {
  const ze_device_memory_properties_t p = {
      ZE_DEVICE_MEMORY_PROPERTIES_VERSION_CURRENT, 1, 2, 3};

  const pt::ptree json = cs::device_memory_properties_to_json(p);

  const std::string actual = cs::ptree_to_string(json);
  const std::string expected = "{\n"
                               "    \"maxClockRate\": 1,\n"
                               "    \"maxBusWidth\": 2,\n"
                               "    \"totalSize\": 3\n"
                               "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceMemoryAccessPropertiesToJSON) {
  const ze_device_memory_access_properties_t p = {
      ZE_DEVICE_MEMORY_ACCESS_PROPERTIES_VERSION_CURRENT,
      static_cast<ze_memory_access_capabilities_t>(ZE_MEMORY_ACCESS_NONE),
      static_cast<ze_memory_access_capabilities_t>(ZE_MEMORY_ACCESS),
      static_cast<ze_memory_access_capabilities_t>(ZE_MEMORY_ATOMIC_ACCESS),
      static_cast<ze_memory_access_capabilities_t>(ZE_MEMORY_CONCURRENT_ACCESS),
      static_cast<ze_memory_access_capabilities_t>(ZE_MEMORY_ACCESS |
                                                   ZE_MEMORY_ATOMIC_ACCESS)};

  const pt::ptree json = cs::device_memory_access_properties_to_json(p);

  const std::string actual = cs::ptree_to_string(json);
  const std::string expected =
      "{\n"
      "    \"hostAllocCapabilities\": [\n"
      "        \"ZE_MEMORY_ACCESS_NONE\"\n"
      "    ],\n"
      "    \"deviceAllocCapabilities\": [\n"
      "        \"ZE_MEMORY_ACCESS\"\n"
      "    ],\n"
      "    \"sharedSingleDeviceAllocCapabilities\": [\n"
      "        \"ZE_MEMORY_ATOMIC_ACCESS\"\n"
      "    ],\n"
      "    \"sharedCrossDeviceAllocCapabilities\": [\n"
      "        \"ZE_MEMORY_CONCURRENT_ACCESS\"\n"
      "    ],\n"
      "    \"sharedSystemAllocCapabilities\": [\n"
      "        \"ZE_MEMORY_ACCESS\",\n"
      "        \"ZE_MEMORY_ATOMIC_ACCESS\"\n"
      "    ]\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceCachePropertiesToJSON) {
  const ze_device_cache_properties_t p = {
      ZE_DEVICE_CACHE_PROPERTIES_VERSION_CURRENT, 1, 2, 3, 1, 4, 5};
  const pt::ptree json = cs::device_cache_properties_to_json(p);

  const std::string actual = cs::ptree_to_string(json);
  const std::string expected =
      "{\n"
      "    \"intermediateCacheControlSupported\": true,\n"
      "    \"intermediateCacheSize\": 2,\n"
      "    \"intermediateCachelineSize\": 3,\n"
      "    \"lastLevelCacheSizeControlSupported\": true,\n"
      "    \"lastLevelCacheSize\": 4,\n"
      "    \"lastLevelCachelineSize\": 5\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceImagePropertiesToJSON) {
  const ze_device_image_properties_t p = {
      ZE_DEVICE_IMAGE_PROPERTIES_VERSION_CURRENT, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  const pt::ptree json = cs::device_image_properties_to_json(p);

  const std::string actual = cs::ptree_to_string(json);
  const std::string expected = "{\n"
                               "    \"supported\": true,\n"
                               "    \"maxImageDims1D\": 2,\n"
                               "    \"maxImageDims2D\": 3,\n"
                               "    \"maxImageDims3D\": 4,\n"
                               "    \"maxImageBufferSize\": 5,\n"
                               "    \"maxImageArraySlices\": 6,\n"
                               "    \"maxSamplers\": 7,\n"
                               "    \"maxReadImageArgs\": 8,\n"
                               "    \"maxWriteImageArgs\": 9\n"
                               "}";
  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, PtreeToStringEmptyArray) {
  pt::ptree children;
  pt::ptree child;
  children.push_back(std::make_pair("", child));

  const std::string actual = cs::ptree_to_string(children);
  const std::string expected = "[]";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, PtreeToStringIntValue) {
  pt::ptree tree;
  tree.put("value", 123);

  const std::string actual = cs::ptree_to_string(tree);
  const std::string expected = "{\n    \"value\": 123\n}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, PtreeToStringFloatValue) {
  pt::ptree tree;
  tree.put("value", 1.5);

  const std::string actual = cs::ptree_to_string(tree);
  const std::string expected = "{\n    \"value\": 1.5\n}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, PtreeToStringTrueValue) {
  pt::ptree tree;
  tree.put("value", true);

  const std::string actual = cs::ptree_to_string(tree);
  const std::string expected = "{\n    \"value\": true\n}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, PtreeToStringFalseValue) {
  pt::ptree tree;
  tree.put("value", false);

  const std::string actual = cs::ptree_to_string(tree);
  const std::string expected = "{\n    \"value\": false\n}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, PtreeToStringTrailingIndentation) {
  pt::ptree tree;
  pt::ptree children;
  pt::ptree child;
  child.put("", "value");
  children.push_back(std::make_pair("", child));
  tree.push_back(std::make_pair("", children));
  tree.push_back(std::make_pair("", children));

  const std::string actual = cs::ptree_to_string(tree);
  const std::string expected = "[\n"
                               "        [\n"
                               "            \"value\"\n"
                               "        ],\n"
                               "        [\n"
                               "            \"value\"\n"
                               "        ]\n"
                               "]";
  EXPECT_THAT(actual, ::testing::StrEq(expected));
}
