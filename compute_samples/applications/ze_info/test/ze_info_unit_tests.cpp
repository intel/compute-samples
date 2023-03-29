/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gmock/gmock.h"
#include "ze_info/text_formatter.hpp"
#include "ze_info/json_formatter.hpp"
#include "ze_api.h"
#include "zet_api.h"

#include <sstream>
#include <cstring>

#include <boost/property_tree/ptree.hpp>

namespace cs = compute_samples;
namespace pt = boost::property_tree;

ze_device_properties_t fake_device_properties() {
  return ze_device_properties_t{
      ZE_STRUCTURE_TYPE_DEVICE_PROPERTIES,
      nullptr,
      ZE_DEVICE_TYPE_GPU,
      1,
      2,
      ZE_DEVICE_PROPERTY_FLAG_ECC | ZE_DEVICE_PROPERTY_FLAG_INTEGRATED,
      0,
      3,
      4,
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
      {0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12,
       0x78, 0x56, 0x34, 0x12},
      "device_name"};
}

ze_device_properties_t fake_sub_device_properties() {
  ze_device_properties_t device_properties = fake_device_properties();
  device_properties.flags =
      device_properties.flags | ZE_DEVICE_PROPERTY_FLAG_SUBDEVICE;
  device_properties.subdeviceId = 15;
  return device_properties;
}

ze_device_compute_properties_t fake_device_compute_properties() {
  return ze_device_compute_properties_t{
      ZE_STRUCTURE_TYPE_DEVICE_COMPUTE_PROPERTIES,
      nullptr,
      1,
      2,
      3,
      4,
      5,
      6,
      7,
      8,
      2,
      {9, 10}};
}

ze_device_module_properties_t fake_device_module_properties() {
  return ze_device_module_properties_t{
      ZE_STRUCTURE_TYPE_MODULE_PROPERTIES,
      nullptr,
      ZE_API_VERSION_1_0,
      ZE_DEVICE_MODULE_FLAG_DP4A | ZE_DEVICE_MODULE_FLAG_FP16,
      ZE_DEVICE_FP_FLAG_DENORM | ZE_DEVICE_FP_FLAG_ROUND_TO_NEAREST,
      ZE_DEVICE_FP_FLAG_INF_NAN | ZE_DEVICE_FP_FLAG_ROUND_TO_ZERO,
      ZE_DEVICE_FP_FLAG_ROUND_TO_INF | ZE_DEVICE_FP_FLAG_ROUNDED_DIVIDE_SQRT,
      2,
      3,
      {0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12,
       0x78, 0x56, 0x34, 0x12}};
}

ze_command_queue_group_properties_t
fake_device_command_queue_group_properties() {
  return ze_command_queue_group_properties_t{
      ZE_STRUCTURE_TYPE_COMMAND_QUEUE_GROUP_PROPERTIES, nullptr,
      ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE |
          ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS,
      1, 2};
}

ze_device_memory_properties_t fake_device_memory_properties() {
  return ze_device_memory_properties_t{
      ZE_STRUCTURE_TYPE_DEVICE_MEMORY_PROPERTIES,
      nullptr,
      ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD,
      1,
      2,
      3,
      "memory_name"};
}

ze_device_memory_access_properties_t fake_device_memory_access_properties() {
  return ze_device_memory_access_properties_t{
      ZE_STRUCTURE_TYPE_DEVICE_MEMORY_ACCESS_PROPERTIES,
      nullptr,
      ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC | ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT,
      ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT,
      ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT_ATOMIC |
          ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT,
      ZE_MEMORY_ACCESS_CAP_FLAG_RW | ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT,
      ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC | ZE_MEMORY_ACCESS_CAP_FLAG_RW};
}

ze_device_cache_properties_t fake_device_cache_properties() {
  return ze_device_cache_properties_t{
      ZE_STRUCTURE_TYPE_DEVICE_CACHE_PROPERTIES, nullptr,
      ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL, 1};
}

ze_device_image_properties_t fake_device_image_properties() {
  return ze_device_image_properties_t{ZE_STRUCTURE_TYPE_DEVICE_IMAGE_PROPERTIES,
                                      nullptr,
                                      1,
                                      2,
                                      3,
                                      4,
                                      5,
                                      6,
                                      7,
                                      8};
}

ze_device_external_memory_properties_t
fake_device_external_memory_properties() {
  return ze_device_external_memory_properties_t{
      ZE_STRUCTURE_TYPE_DEVICE_EXTERNAL_MEMORY_PROPERTIES,
      nullptr,
      ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF |
          ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD,
      ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF |
          ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD,
      ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF |
          ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD,
      ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF |
          ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD};
}

zet_device_debug_properties_t fake_device_debug_properties() {
  return zet_device_debug_properties_t{
      ZET_STRUCTURE_TYPE_DEVICE_DEBUG_PROPERTIES, nullptr,
      ZET_DEVICE_DEBUG_PROPERTY_FLAG_ATTACH};
}

ze_scheduling_hint_exp_properties_t
fake_device_kernel_schedule_hint_properties() {
  return ze_scheduling_hint_exp_properties_t{
      ZE_STRUCTURE_TYPE_SCHEDULING_HINT_EXP_PROPERTIES, nullptr,
      ZE_SCHEDULING_HINT_EXP_FLAG_ROUND_ROBIN |
          ZE_SCHEDULING_HINT_EXP_FLAG_STALL_BASED_ROUND_ROBIN};
}

ze_float_atomic_ext_properties_t fake_float_atomic_ext_properties() {
  return ze_float_atomic_ext_properties_t{
      ZE_STRUCTURE_TYPE_FLOAT_ATOMIC_EXT_PROPERTIES, nullptr,
      ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE |
          ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_ADD |
          ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX,
      ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE |
          ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX,
      ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE};
}

ze_device_raytracing_ext_properties_t fake_raytracing_ext_properties() {
  return ze_device_raytracing_ext_properties_t{
      ZE_STRUCTURE_TYPE_DEVICE_RAYTRACING_EXT_PROPERTIES, nullptr,
      ZE_DEVICE_RAYTRACING_EXT_FLAG_RAYQUERY, 1};
}

cs::DeviceCapabilities fake_device_capabilities() {
  cs::DeviceCapabilities capabilities = {};
  capabilities.device_properties = fake_device_properties();
  capabilities.compute_properties = fake_device_compute_properties();
  capabilities.module_properties = fake_device_module_properties();
  capabilities.scheduling_hint_properties =
      fake_device_kernel_schedule_hint_properties();
  capabilities.float_atomics_properties = fake_float_atomic_ext_properties();
  capabilities.ray_tracing_properties = fake_raytracing_ext_properties();
  capabilities.command_queue_group_properties = {
      fake_device_command_queue_group_properties(),
      fake_device_command_queue_group_properties()};
  capabilities.memory_properties = {fake_device_memory_properties(),
                                    fake_device_memory_properties()};
  capabilities.memory_access_properties =
      fake_device_memory_access_properties();
  capabilities.cache_properties = {fake_device_cache_properties(),
                                   fake_device_cache_properties()};
  capabilities.image_properties = fake_device_image_properties();
  capabilities.external_memory_properties =
      fake_device_external_memory_properties();
  capabilities.debug_properties = fake_device_debug_properties();
  return capabilities;
}

cs::DeviceCapabilities fake_sub_device_capabilities() {
  cs::DeviceCapabilities capabilities = fake_device_capabilities();
  capabilities.device_properties = fake_sub_device_properties();
  return capabilities;
}

cs::DeviceCapabilities fake_device_with_sub_devices_capabilities() {
  cs::DeviceCapabilities capabilities = fake_device_capabilities();
  capabilities.sub_devices = {fake_sub_device_capabilities(),
                              fake_sub_device_capabilities()};
  return capabilities;
}

ze_api_version_t fake_driver_api_version() { return ZE_API_VERSION_1_0; }

ze_driver_properties_t fake_driver_properties() {
  return ze_driver_properties_t{ZE_STRUCTURE_TYPE_DRIVER_PROPERTIES,
                                nullptr,
                                {0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34, 0x12,
                                 0x78, 0x56, 0x34, 0x12, 0x78, 0x56, 0x34,
                                 0x12},
                                123};
}

ze_driver_ipc_properties_t fake_driver_ipc_properties() {
  return ze_driver_ipc_properties_t{
      ZE_STRUCTURE_TYPE_DRIVER_IPC_PROPERTIES, nullptr,
      ZE_IPC_PROPERTY_FLAG_EVENT_POOL | ZE_IPC_PROPERTY_FLAG_MEMORY};
}

ze_driver_extension_properties_t fake_driver_extension_properties() {
  return ze_driver_extension_properties_t{"extension_a", ZE_API_VERSION_1_0};
}

cs::DriverCapabilities fake_driver_capabilities() {
  cs::DriverCapabilities driver_capabilities = {};
  driver_capabilities.api_version = fake_driver_api_version();
  driver_capabilities.driver_properties = fake_driver_properties();
  driver_capabilities.ipc_properties = fake_driver_ipc_properties();
  driver_capabilities.extension_properties = {
      fake_driver_extension_properties(),
      ze_driver_extension_properties_t{"extension_b", ZE_API_VERSION_1_0}};
  return driver_capabilities;
}

cs::DriverCapabilities fake_driver_with_devices_capabilities() {
  cs::DriverCapabilities driver_capabilities = fake_driver_capabilities();
  driver_capabilities.devices = {fake_device_capabilities(),
                                 fake_device_capabilities()};
  return driver_capabilities;
}

cs::DriverCapabilities
fake_driver_with_devices_with_sub_devices_capabilities() {
  cs::DriverCapabilities driver_capabilities = fake_driver_capabilities();
  driver_capabilities.devices = {fake_device_with_sub_devices_capabilities(),
                                 fake_device_with_sub_devices_capabilities()};
  return driver_capabilities;
}

class TextFormatterTests : public ::testing::Test {
protected:
  const int indentation_level_ = 1;
};

TEST_F(TextFormatterTests, DriversCapabilitiesToText) {
  const auto capabilities = std::vector<cs::DriverCapabilities>{
      fake_driver_capabilities(), fake_driver_capabilities()};

  std::stringstream ss;
  ss << cs::key_value_to_text("Number of drivers", "2", indentation_level_);
  ss << cs::key_value_to_text("Driver", "0", indentation_level_);
  ss << cs::driver_capabilities_to_text(capabilities[0],
                                        indentation_level_ + 1);
  ss << cs::key_value_to_text("Driver", "1", indentation_level_);
  ss << cs::driver_capabilities_to_text(capabilities[1],
                                        indentation_level_ + 1);
  const auto expected = ss.str();
  const auto actual =
      cs::drivers_capabilities_to_text(capabilities, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DriverCapabilitiesToText) {
  const auto capabilities = fake_driver_capabilities();

  std::stringstream ss;
  ss << cs::driver_api_version_to_text(capabilities.api_version,
                                       indentation_level_);
  ss << cs::driver_properties_to_text(capabilities.driver_properties,
                                      indentation_level_);
  ss << cs::driver_ipc_properties_to_text(capabilities.ipc_properties,
                                          indentation_level_);
  ss << cs::all_driver_extension_properties_to_text(
      capabilities.extension_properties, indentation_level_);
  ss << cs::key_value_to_text("Number of devices", "0", indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::driver_capabilities_to_text(capabilities, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DriverWithDevicesCapabilitiesToText) {
  const auto capabilities = fake_driver_with_devices_capabilities();

  std::stringstream ss;
  ss << cs::driver_api_version_to_text(capabilities.api_version,
                                       indentation_level_);
  ss << cs::driver_properties_to_text(capabilities.driver_properties,
                                      indentation_level_);
  ss << cs::driver_ipc_properties_to_text(capabilities.ipc_properties,
                                          indentation_level_);
  ss << cs::all_driver_extension_properties_to_text(
      capabilities.extension_properties, indentation_level_);
  ss << cs::key_value_to_text("Number of devices", "2", indentation_level_);
  ss << cs::key_value_to_text("Device", "0", indentation_level_);
  ss << cs::device_capabilities_to_text(capabilities.devices[0],
                                        indentation_level_ + 1);
  ss << cs::key_value_to_text("Device", "1", indentation_level_);
  ss << cs::device_capabilities_to_text(capabilities.devices[1],
                                        indentation_level_ + 1);
  const auto expected = ss.str();
  const auto actual =
      cs::driver_capabilities_to_text(capabilities, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DriverApiVersionToText) {
  const auto api_version = fake_driver_api_version();

  std::stringstream ss;
  ss << cs::key_value_to_text("Driver API version", "1.0", indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::driver_api_version_to_text(api_version, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DriverPropertiesToText) {
  const auto properties = fake_driver_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text("Driver version", "123", indentation_level_);
  ss << cs::key_value_to_text("UUID", "12345678-1234-5678-1234-567812345678",
                              indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::driver_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DriverIpcPropertiesToText) {
  const auto properties = fake_driver_ipc_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text(
      "IPC flags",
      "ZE_IPC_PROPERTY_FLAG_EVENT_POOL | ZE_IPC_PROPERTY_FLAG_MEMORY",
      indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::driver_ipc_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, AllDriverExtensionPropertiesToText) {
  const auto properties = std::vector<ze_driver_extension_properties_t>{
      fake_driver_extension_properties(),
      ze_driver_extension_properties_t{"extension_b", ZE_API_VERSION_1_0}};

  std::stringstream ss;
  ss << cs::key_value_to_text("Number of extensions", "2", indentation_level_);
  ss << cs::key_value_to_text("Extension", "0", indentation_level_);
  ss << cs::driver_extension_properties_to_text(properties[0],
                                                indentation_level_ + 1);
  ss << cs::key_value_to_text("Extension", "1", indentation_level_);
  ss << cs::driver_extension_properties_to_text(properties[1],
                                                indentation_level_ + 1);
  const auto expected = ss.str();
  const auto actual = cs::all_driver_extension_properties_to_text(
      properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DriverExtensionPropertiesToText) {
  const auto properties = fake_driver_extension_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text("Name", "extension_a", indentation_level_);
  ss << cs::key_value_to_text("Version", "1.0", indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::driver_extension_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceCapabilitiesToText) {
  const auto capabilities = fake_device_with_sub_devices_capabilities();

  std::stringstream ss;
  ss << cs::device_properties_to_text(capabilities.device_properties,
                                      indentation_level_);
  ss << cs::device_compute_properties_to_text(capabilities.compute_properties,
                                              indentation_level_);
  ss << cs::device_module_properties_to_text(capabilities.module_properties,
                                             indentation_level_);
  ss << cs::kernel_scheduling_hint_properties_to_text(
      capabilities.scheduling_hint_properties, indentation_level_);
  ss << cs::float_atomics_properties_to_text(
      capabilities.float_atomics_properties, indentation_level_);
  ss << cs::device_raytracing_properties_to_text(
      capabilities.ray_tracing_properties, indentation_level_);
  ss << cs::all_device_command_queue_group_properties_to_text(
      capabilities.command_queue_group_properties, indentation_level_);
  ss << cs::all_device_memory_properties_to_text(capabilities.memory_properties,
                                                 indentation_level_);
  ss << cs::device_memory_access_properties_to_text(
      capabilities.memory_access_properties, indentation_level_);
  ss << cs::all_device_cache_properties_to_text(capabilities.cache_properties,
                                                indentation_level_);
  ss << cs::device_image_properties_to_text(capabilities.image_properties,
                                            indentation_level_);
  ss << cs::device_external_memory_properties_to_text(
      capabilities.external_memory_properties, indentation_level_);
  ss << cs::device_debug_properties_to_text(capabilities.debug_properties,
                                            indentation_level_);
  ss << cs::key_value_to_text("Number of sub-devices", "2", indentation_level_);
  ss << cs::key_value_to_text("Sub-device", "0", indentation_level_);
  ss << cs::device_capabilities_to_text(capabilities.sub_devices[0],
                                        indentation_level_ + 1);
  ss << cs::key_value_to_text("Sub-device", "1", indentation_level_);
  ss << cs::device_capabilities_to_text(capabilities.sub_devices[1],
                                        indentation_level_ + 1);
  const auto expected = ss.str();
  const auto actual =
      cs::device_capabilities_to_text(capabilities, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DevicePropertiesToText) {
  const auto properties = fake_device_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text("Name", "device_name", indentation_level_);
  ss << cs::key_value_to_text("Type", "ZE_DEVICE_TYPE_GPU", indentation_level_);
  ss << cs::key_value_to_text("Vendor ID", "1", indentation_level_);
  ss << cs::key_value_to_text("Device ID", "2", indentation_level_);
  ss << cs::key_value_to_text("UUID", "12345678-1234-5678-1234-567812345678",
                              indentation_level_);
  ss << cs::key_value_to_text(
      "Device flags",
      "ZE_DEVICE_PROPERTY_FLAG_ECC | ZE_DEVICE_PROPERTY_FLAG_INTEGRATED",
      indentation_level_);
  ss << cs::key_value_to_text("Core clock rate", "3", indentation_level_);
  ss << cs::key_value_to_text("Maximum memory allocation size", "4",
                              indentation_level_);
  ss << cs::key_value_to_text("Maximum number of logical hardware contexts",
                              "5", indentation_level_);
  ss << cs::key_value_to_text("Maximum priority for command queues", "6",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of threads per EU", "7",
                              indentation_level_);
  ss << cs::key_value_to_text("Physical EU SIMD width", "8",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of EUs per subslice", "9",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of subslices per slice", "10",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of slices", "11", indentation_level_);
  ss << cs::key_value_to_text("Timer resolution", "12", indentation_level_);
  ss << cs::key_value_to_text("Timestamp valid bits", "13", indentation_level_);
  ss << cs::key_value_to_text("Kernel timestamp valid bits", "14",
                              indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::device_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, SubDevicePropertiesToText) {
  const auto properties = fake_sub_device_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text("Name", "device_name", indentation_level_);
  ss << cs::key_value_to_text("Type", "ZE_DEVICE_TYPE_GPU", indentation_level_);
  ss << cs::key_value_to_text("Vendor ID", "1", indentation_level_);
  ss << cs::key_value_to_text("Device ID", "2", indentation_level_);
  ss << cs::key_value_to_text("UUID", "12345678-1234-5678-1234-567812345678",
                              indentation_level_);
  ss << cs::key_value_to_text(
      "Device flags",
      "ZE_DEVICE_PROPERTY_FLAG_ECC | ZE_DEVICE_PROPERTY_FLAG_INTEGRATED "
      "| ZE_DEVICE_PROPERTY_FLAG_SUBDEVICE",
      indentation_level_);
  ss << cs::key_value_to_text("Sub-device ID", "15", indentation_level_);
  ss << cs::key_value_to_text("Core clock rate", "3", indentation_level_);
  ss << cs::key_value_to_text("Maximum memory allocation size", "4",
                              indentation_level_);
  ss << cs::key_value_to_text("Maximum number of logical hardware contexts",
                              "5", indentation_level_);
  ss << cs::key_value_to_text("Maximum priority for command queues", "6",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of threads per EU", "7",
                              indentation_level_);
  ss << cs::key_value_to_text("Physical EU SIMD width", "8",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of EUs per subslice", "9",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of subslices per slice", "10",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of slices", "11", indentation_level_);
  ss << cs::key_value_to_text("Timer resolution", "12", indentation_level_);
  ss << cs::key_value_to_text("Timestamp valid bits", "13", indentation_level_);
  ss << cs::key_value_to_text("Kernel timestamp valid bits", "14",
                              indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::device_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceComputePropertiesToText) {
  const auto properties = fake_device_compute_properties();

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
  ss << cs::key_value_to_text("Sub-group sizes", "9 10", indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::device_compute_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceModulePropertiesToText) {
  const auto properties = fake_device_module_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text("SPIR-V version", "1.0", indentation_level_);
  ss << cs::key_value_to_text(
      "Module flags", "ZE_DEVICE_MODULE_FLAG_DP4A | ZE_DEVICE_MODULE_FLAG_FP16",
      indentation_level_);
  ss << cs::key_value_to_text(
      "FP16 flags",
      "ZE_DEVICE_FP_FLAG_DENORM | ZE_DEVICE_FP_FLAG_ROUND_TO_NEAREST",
      indentation_level_);
  ss << cs::key_value_to_text(
      "FP32 flags",
      "ZE_DEVICE_FP_FLAG_INF_NAN | ZE_DEVICE_FP_FLAG_ROUND_TO_ZERO",
      indentation_level_);
  ss << cs::key_value_to_text(
      "FP64 flags",
      "ZE_DEVICE_FP_FLAG_ROUNDED_DIVIDE_SQRT | ZE_DEVICE_FP_FLAG_ROUND_TO_INF",
      indentation_level_);
  ss << cs::key_value_to_text("Maximum argument size", "2", indentation_level_);
  ss << cs::key_value_to_text("printf buffer size", "3", indentation_level_);
  ss << cs::key_value_to_text("Native kernel supported",
                              "12345678-1234-5678-1234-567812345678",
                              indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::device_module_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, AllDeviceCommandQueueGroupPropertiesToText) {
  const auto properties = std::vector<ze_command_queue_group_properties_t>{
      fake_device_command_queue_group_properties(),
      fake_device_command_queue_group_properties()};

  std::stringstream ss;
  ss << cs::key_value_to_text("Number of command queue groups", "2",
                              indentation_level_);
  ss << cs::key_value_to_text("Command queue group", "0", indentation_level_);
  ss << cs::device_command_queue_group_properties_to_text(
      properties[0], indentation_level_ + 1);
  ss << cs::key_value_to_text("Command queue group", "1", indentation_level_);
  ss << cs::device_command_queue_group_properties_to_text(
      properties[1], indentation_level_ + 1);
  const auto expected = ss.str();
  const auto actual = cs::all_device_command_queue_group_properties_to_text(
      properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceCommandQueueGroupPropertiesToText) {
  const auto properties = fake_device_command_queue_group_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text(
      "Command queue group flags",
      "ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE | "
      "ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS",
      indentation_level_);
  ss << cs::key_value_to_text("Maximum fill pattern size", "1",
                              indentation_level_);
  ss << cs::key_value_to_text("Number of physical command queues", "2",
                              indentation_level_);
  const auto expected = ss.str();
  const auto actual = cs::device_command_queue_group_properties_to_text(
      properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, AllDeviceMemoryPropertiesToText) {
  const auto properties = std::vector<ze_device_memory_properties_t>{
      fake_device_memory_properties(), fake_device_memory_properties()};

  std::stringstream ss;
  ss << cs::key_value_to_text("Number of memory properties", "2",
                              indentation_level_);
  ss << cs::key_value_to_text("Memory", "0", indentation_level_);
  ss << cs::device_memory_properties_to_text(properties[0],
                                             indentation_level_ + 1);
  ss << cs::key_value_to_text("Memory", "1", indentation_level_);
  ss << cs::device_memory_properties_to_text(properties[1],
                                             indentation_level_ + 1);
  const auto expected = ss.str();
  const auto actual =
      cs::all_device_memory_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceMemoryPropertiesToText) {
  const auto properties = fake_device_memory_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text("Name", "memory_name", indentation_level_);
  ss << cs::key_value_to_text(
      "Memory flags", "ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD", indentation_level_);
  ss << cs::key_value_to_text("Maximum clock rate", "1", indentation_level_);
  ss << cs::key_value_to_text("Maximum bus width", "2", indentation_level_);
  ss << cs::key_value_to_text("Total size", "3", indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::device_memory_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceMemoryAccessPropertiesToText) {
  const auto properties = fake_device_memory_access_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text(
      "Host allocation capabilities",
      "ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC | ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT",
      indentation_level_);
  ss << cs::key_value_to_text("Device allocation capabilities",
                              "ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT",
                              indentation_level_);
  ss << cs::key_value_to_text("Shared single-device allocation capabilities",
                              "ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT | "
                              "ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT_ATOMIC",
                              indentation_level_);
  ss << cs::key_value_to_text(
      "Shared cross-device allocation capabilities",
      "ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT | ZE_MEMORY_ACCESS_CAP_FLAG_RW",
      indentation_level_);
  ss << cs::key_value_to_text(
      "Shared system allocation capabilities",
      "ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC | ZE_MEMORY_ACCESS_CAP_FLAG_RW",
      indentation_level_);
  const auto expected = ss.str();
  const auto actual = cs::device_memory_access_properties_to_text(
      properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, AllDeviceCachePropertiesToText) {
  const auto properties = std::vector<ze_device_cache_properties_t>{
      fake_device_cache_properties(), fake_device_cache_properties()};

  std::stringstream ss;
  ss << cs::key_value_to_text("Number of cache properties", "2",
                              indentation_level_);
  ss << cs::key_value_to_text("Cache", "0", indentation_level_);
  ss << cs::device_cache_properties_to_text(properties[0],
                                            indentation_level_ + 1);
  ss << cs::key_value_to_text("Cache", "1", indentation_level_);
  ss << cs::device_cache_properties_to_text(properties[1],
                                            indentation_level_ + 1);
  const auto expected = ss.str();
  const auto actual =
      cs::all_device_cache_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceCachePropertiesToText) {
  const auto properties = fake_device_cache_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text("Cache flags",
                              "ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL",
                              indentation_level_);
  ss << cs::key_value_to_text("Cache size", "1", indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::device_cache_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceImagePropertiesToText) {
  const auto properties = fake_device_image_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text("Maximum image 1D", "1", indentation_level_);
  ss << cs::key_value_to_text("Maximum image 2D", "2", indentation_level_);
  ss << cs::key_value_to_text("Maximum image 3D", "3", indentation_level_);
  ss << cs::key_value_to_text("Maximum image buffer size", "4",
                              indentation_level_);
  ss << cs::key_value_to_text("Maximum image array slices", "5",
                              indentation_level_);
  ss << cs::key_value_to_text("Maximum number of samplers", "6",
                              indentation_level_);
  ss << cs::key_value_to_text("Maximum number of read arguments", "7",
                              indentation_level_);
  ss << cs::key_value_to_text("Maximum number of write arguments", "8",
                              indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::device_image_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceExternalMemoryPropertiesToText) {
  const auto properties = fake_device_external_memory_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text("Memory allocation import types",
                              "ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF | "
                              "ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD",
                              indentation_level_);
  ss << cs::key_value_to_text("Memory allocation import types",
                              "ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF | "
                              "ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD",
                              indentation_level_);
  ss << cs::key_value_to_text("Image import types",
                              "ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF | "
                              "ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD",
                              indentation_level_);
  ss << cs::key_value_to_text("Image export types",
                              "ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF | "
                              "ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD",
                              indentation_level_);
  const auto expected = ss.str();
  const auto actual = cs::device_external_memory_properties_to_text(
      properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, DeviceDebugPropertiesToText) {
  const auto properties = fake_device_debug_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text("flags", "ZET_DEVICE_DEBUG_PROPERTY_FLAG_ATTACH",
                              indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::device_debug_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, KeyValueToText) {
  const std::string actual =
      cs::key_value_to_text("Key", "Value", indentation_level_);

  const std::string expected = "    Key                                        "
                               "                                 Value\n";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, KernelHintsPropertiesToText) {
  const auto properties = fake_device_kernel_schedule_hint_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text(
      "Kernel scheduling hints flags",
      "ZE_SCHEDULING_HINT_EXP_FLAG_ROUND_ROBIN | "
      "ZE_SCHEDULING_HINT_EXP_FLAG_STALL_BASED_ROUND_ROBIN",
      indentation_level_);
  const auto expected = ss.str();
  const auto actual = cs::kernel_scheduling_hint_properties_to_text(
      properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, FloatAtomicsPropertiesToText) {
  const auto properties = fake_float_atomic_ext_properties();
  std::stringstream ss;
  ss << cs::key_value_to_text(
      "Float atomics FP16 flags",
      "ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_ADD "
      "| ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE | "
      "ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX",
      indentation_level_);
  ss << cs::key_value_to_text("Float atomics FP32 flags",
                              "ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE "
                              "| ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX",
                              indentation_level_);
  ss << cs::key_value_to_text("Float atomics FP64 flags",
                              "ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE",
                              indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::float_atomics_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST_F(TextFormatterTests, RaytracingPropertiesToText) {
  const auto properties = fake_raytracing_ext_properties();

  std::stringstream ss;
  ss << cs::key_value_to_text("Raytracing properties",
                              "ZE_DEVICE_RAYTRACING_EXT_FLAG_RAYQUERY",
                              indentation_level_);
  ss << cs::key_value_to_text("Raytracing maxBVHLevels", "1",
                              indentation_level_);
  const auto expected = ss.str();
  const auto actual =
      cs::device_raytracing_properties_to_text(properties, indentation_level_);

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DriversCapabilitiesToJSON) {
  const auto capabilities = std::vector<cs::DriverCapabilities>{
      fake_driver_capabilities(), fake_driver_capabilities()};

  const auto json = cs::drivers_capabilities_to_json(capabilities);
  const auto actual = cs::ptree_to_string(json);
  const auto expected =
      "[\n"
      "        {\n"
      "            \"ze_api_version_t\": \"1.0\",\n"
      "            \"ze_driver_properties_t\": {\n"
      "                \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "                \"driverVersion\": 123\n"
      "            },\n"
      "            \"ze_driver_ipc_properties_t\": {\n"
      "                \"flags\": [\n"
      "                    \"ZE_IPC_PROPERTY_FLAG_EVENT_POOL\",\n"
      "                    \"ZE_IPC_PROPERTY_FLAG_MEMORY\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_driver_extension_properties_t\": [\n"
      "                {\n"
      "                    \"name\": \"extension_a\",\n"
      "                    \"version\": \"1.0\"\n"
      "                },\n"
      "                {\n"
      "                    \"name\": \"extension_b\",\n"
      "                    \"version\": \"1.0\"\n"
      "                }\n"
      "            ],\n"
      "            \"devices\": []\n"
      "        },\n"
      "        {\n"
      "            \"ze_api_version_t\": \"1.0\",\n"
      "            \"ze_driver_properties_t\": {\n"
      "                \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "                \"driverVersion\": 123\n"
      "            },\n"
      "            \"ze_driver_ipc_properties_t\": {\n"
      "                \"flags\": [\n"
      "                    \"ZE_IPC_PROPERTY_FLAG_EVENT_POOL\",\n"
      "                    \"ZE_IPC_PROPERTY_FLAG_MEMORY\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_driver_extension_properties_t\": [\n"
      "                {\n"
      "                    \"name\": \"extension_a\",\n"
      "                    \"version\": \"1.0\"\n"
      "                },\n"
      "                {\n"
      "                    \"name\": \"extension_b\",\n"
      "                    \"version\": \"1.0\"\n"
      "                }\n"
      "            ],\n"
      "            \"devices\": []\n"
      "        }\n"
      "]";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DriverCapabilitiesToJSON) {
  const auto capabilities = fake_driver_capabilities();

  const auto json = cs::driver_capabilities_to_json(capabilities);
  const auto actual = cs::ptree_to_string(json);
  const auto expected =
      "{\n"
      "    \"ze_api_version_t\": \"1.0\",\n"
      "    \"ze_driver_properties_t\": {\n"
      "        \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "        \"driverVersion\": 123\n"
      "    },\n"
      "    \"ze_driver_ipc_properties_t\": {\n"
      "        \"flags\": [\n"
      "            \"ZE_IPC_PROPERTY_FLAG_EVENT_POOL\",\n"
      "            \"ZE_IPC_PROPERTY_FLAG_MEMORY\"\n"
      "        ]\n"
      "    },\n"
      "    \"ze_driver_extension_properties_t\": [\n"
      "        {\n"
      "            \"name\": \"extension_a\",\n"
      "            \"version\": \"1.0\"\n"
      "        },\n"
      "        {\n"
      "            \"name\": \"extension_b\",\n"
      "            \"version\": \"1.0\"\n"
      "        }\n"
      "    ],\n"
      "    \"devices\": []\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DriverWithDevicesCapabilitiesToJSON) {
  const auto capabilities = fake_driver_with_devices_capabilities();

  const auto json = cs::driver_capabilities_to_json(capabilities);
  const auto actual = cs::ptree_to_string(json);
  const auto expected =
      "{\n"
      "    \"ze_api_version_t\": \"1.0\",\n"
      "    \"ze_driver_properties_t\": {\n"
      "        \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "        \"driverVersion\": 123\n"
      "    },\n"
      "    \"ze_driver_ipc_properties_t\": {\n"
      "        \"flags\": [\n"
      "            \"ZE_IPC_PROPERTY_FLAG_EVENT_POOL\",\n"
      "            \"ZE_IPC_PROPERTY_FLAG_MEMORY\"\n"
      "        ]\n"
      "    },\n"
      "    \"ze_driver_extension_properties_t\": [\n"
      "        {\n"
      "            \"name\": \"extension_a\",\n"
      "            \"version\": \"1.0\"\n"
      "        },\n"
      "        {\n"
      "            \"name\": \"extension_b\",\n"
      "            \"version\": \"1.0\"\n"
      "        }\n"
      "    ],\n"
      "    \"devices\": [\n"
      "        {\n"
      "            \"ze_device_properties_t\": {\n"
      "                \"name\": \"device_name\",\n"
      "                \"type\": \"ZE_DEVICE_TYPE_GPU\",\n"
      "                \"vendorId\": 1,\n"
      "                \"deviceId\": 2,\n"
      "                \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "                \"flags\": [\n"
      "                    \"ZE_DEVICE_PROPERTY_FLAG_ECC\",\n"
      "                    \"ZE_DEVICE_PROPERTY_FLAG_INTEGRATED\"\n"
      "                ],\n"
      "                \"coreClockRate\": 3,\n"
      "                \"maxMemAllocSize\": 4,\n"
      "                \"maxHardwareContexts\": 5,\n"
      "                \"maxCommandQueuePriority\": 6,\n"
      "                \"numThreadsPerEU\": 7,\n"
      "                \"physicalEUSimdWidth\": 8,\n"
      "                \"numEUsPerSubslice\": 9,\n"
      "                \"numSubslicesPerSlice\": 10,\n"
      "                \"numSlices\": 11,\n"
      "                \"timerResolution\": 12,\n"
      "                \"timestampValidBits\": 13,\n"
      "                \"kernelTimestampValidBits\": 14\n"
      "            },\n"
      "            \"ze_device_compute_properties_t\": {\n"
      "                \"maxTotalGroupSize\": 1,\n"
      "                \"maxGroupSizeX\": 2,\n"
      "                \"maxGroupSizeY\": 3,\n"
      "                \"maxGroupSizeZ\": 4,\n"
      "                \"maxGroupCountX\": 5,\n"
      "                \"maxGroupCountY\": 6,\n"
      "                \"maxGroupCountZ\": 7,\n"
      "                \"maxSharedLocalMemory\": 8,\n"
      "                \"subGroupSizes\": [\n"
      "                    9,\n"
      "                    10\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_module_properties_t\": {\n"
      "                \"spirvVersionSupported\": \"1.0\",\n"
      "                \"flags\": [\n"
      "                    \"ZE_DEVICE_MODULE_FLAG_DP4A\",\n"
      "                    \"ZE_DEVICE_MODULE_FLAG_FP16\"\n"
      "                ],\n"
      "                \"fp16flags\": [\n"
      "                    \"ZE_DEVICE_FP_FLAG_DENORM\",\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUND_TO_NEAREST\"\n"
      "                ],\n"
      "                \"fp32flags\": [\n"
      "                    \"ZE_DEVICE_FP_FLAG_INF_NAN\",\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUND_TO_ZERO\"\n"
      "                ],\n"
      "                \"fp64flags\": [\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUNDED_DIVIDE_SQRT\",\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUND_TO_INF\"\n"
      "                ],\n"
      "                \"maxArgumentsSize\": 2,\n"
      "                \"printfBufferSize\": 3,\n"
      "                \"nativeKernelSupported\": "
      "\"12345678-1234-5678-1234-567812345678\"\n"
      "            },\n"
      "            \"ze_command_queue_group_properties_t\": [\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE\",\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS\"\n"
      "                    ],\n"
      "                    \"maxMemoryFillPatternSize\": 1,\n"
      "                    \"numQueues\": 2\n"
      "                },\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE\",\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS\"\n"
      "                    ],\n"
      "                    \"maxMemoryFillPatternSize\": 1,\n"
      "                    \"numQueues\": 2\n"
      "                }\n"
      "            ],\n"
      "            \"ze_scheduling_hint_exp_properties_t\": {\n"
      "                \"schedulingHintFlags\": [\n"
      "                    \"ZE_SCHEDULING_HINT_EXP_FLAG_ROUND_ROBIN\",\n"
      "                    "
      "\"ZE_SCHEDULING_HINT_EXP_FLAG_STALL_BASED_ROUND_ROBIN\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_float_atomic_ext_properties_t\": {\n"
      "                \"fp16Flags\": [\n"
      "                    "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_ADD\",\n"
      "                    "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\",\n"
      "                    \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX\"\n"
      "                ],\n"
      "                \"fp32Flags\": [\n"
      "                    "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\",\n"
      "                    \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX\"\n"
      "                ],\n"
      "                \"fp64Flags\": [\n"
      "                    \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_raytracing_ext_properties_t\": {\n"
      "                \"flags\": [\n"
      "                    \"ZE_DEVICE_RAYTRACING_EXT_FLAG_RAYQUERY\"\n"
      "                ],\n"
      "                \"maxBVHLevels\": 1\n"
      "            },\n"
      "            \"ze_device_memory_properties_t\": [\n"
      "                {\n"
      "                    \"name\": \"memory_name\",\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD\"\n"
      "                    ],\n"
      "                    \"maxClockRate\": 1,\n"
      "                    \"maxBusWidth\": 2,\n"
      "                    \"totalSize\": 3\n"
      "                },\n"
      "                {\n"
      "                    \"name\": \"memory_name\",\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD\"\n"
      "                    ],\n"
      "                    \"maxClockRate\": 1,\n"
      "                    \"maxBusWidth\": 2,\n"
      "                    \"totalSize\": 3\n"
      "                }\n"
      "            ],\n"
      "            \"ze_device_memory_access_properties_t\": {\n"
      "                \"hostAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\"\n"
      "                ],\n"
      "                \"deviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\"\n"
      "                ],\n"
      "                \"sharedSingleDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT_ATOMIC\"\n"
      "                ],\n"
      "                \"sharedCrossDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_RW\"\n"
      "                ],\n"
      "                \"sharedSystemAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_RW\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_cache_properties_t\": [\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL\"\n"
      "                    ],\n"
      "                    \"cacheSize\": 1\n"
      "                },\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL\"\n"
      "                    ],\n"
      "                    \"cacheSize\": 1\n"
      "                }\n"
      "            ],\n"
      "            \"ze_device_image_properties_t\": {\n"
      "                \"maxImageDims1D\": 1,\n"
      "                \"maxImageDims2D\": 2,\n"
      "                \"maxImageDims3D\": 3,\n"
      "                \"maxImageBufferSize\": 4,\n"
      "                \"maxImageArraySlices\": 5,\n"
      "                \"maxSamplers\": 6,\n"
      "                \"maxReadImageArgs\": 7,\n"
      "                \"maxWriteImageArgs\": 8\n"
      "            },\n"
      "            \"ze_device_external_memory_properties_t\": {\n"
      "                \"memoryAllocationImportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ],\n"
      "                \"memoryAllocationExportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ],\n"
      "                \"imageImportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ],\n"
      "                \"imageExportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ]\n"
      "            },\n"
      "            \"zet_device_debug_properties_t\": {\n"
      "                \"flags\": [\n"
      "                    \"ZET_DEVICE_DEBUG_PROPERTY_FLAG_ATTACH\"\n"
      "                ]\n"
      "            },\n"
      "            \"sub_devices_count\": 0,\n"
      "            \"sub_devices\": []\n"
      "        },\n"
      "        {\n"
      "            \"ze_device_properties_t\": {\n"
      "                \"name\": \"device_name\",\n"
      "                \"type\": \"ZE_DEVICE_TYPE_GPU\",\n"
      "                \"vendorId\": 1,\n"
      "                \"deviceId\": 2,\n"
      "                \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "                \"flags\": [\n"
      "                    \"ZE_DEVICE_PROPERTY_FLAG_ECC\",\n"
      "                    \"ZE_DEVICE_PROPERTY_FLAG_INTEGRATED\"\n"
      "                ],\n"
      "                \"coreClockRate\": 3,\n"
      "                \"maxMemAllocSize\": 4,\n"
      "                \"maxHardwareContexts\": 5,\n"
      "                \"maxCommandQueuePriority\": 6,\n"
      "                \"numThreadsPerEU\": 7,\n"
      "                \"physicalEUSimdWidth\": 8,\n"
      "                \"numEUsPerSubslice\": 9,\n"
      "                \"numSubslicesPerSlice\": 10,\n"
      "                \"numSlices\": 11,\n"
      "                \"timerResolution\": 12,\n"
      "                \"timestampValidBits\": 13,\n"
      "                \"kernelTimestampValidBits\": 14\n"
      "            },\n"
      "            \"ze_device_compute_properties_t\": {\n"
      "                \"maxTotalGroupSize\": 1,\n"
      "                \"maxGroupSizeX\": 2,\n"
      "                \"maxGroupSizeY\": 3,\n"
      "                \"maxGroupSizeZ\": 4,\n"
      "                \"maxGroupCountX\": 5,\n"
      "                \"maxGroupCountY\": 6,\n"
      "                \"maxGroupCountZ\": 7,\n"
      "                \"maxSharedLocalMemory\": 8,\n"
      "                \"subGroupSizes\": [\n"
      "                    9,\n"
      "                    10\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_module_properties_t\": {\n"
      "                \"spirvVersionSupported\": \"1.0\",\n"
      "                \"flags\": [\n"
      "                    \"ZE_DEVICE_MODULE_FLAG_DP4A\",\n"
      "                    \"ZE_DEVICE_MODULE_FLAG_FP16\"\n"
      "                ],\n"
      "                \"fp16flags\": [\n"
      "                    \"ZE_DEVICE_FP_FLAG_DENORM\",\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUND_TO_NEAREST\"\n"
      "                ],\n"
      "                \"fp32flags\": [\n"
      "                    \"ZE_DEVICE_FP_FLAG_INF_NAN\",\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUND_TO_ZERO\"\n"
      "                ],\n"
      "                \"fp64flags\": [\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUNDED_DIVIDE_SQRT\",\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUND_TO_INF\"\n"
      "                ],\n"
      "                \"maxArgumentsSize\": 2,\n"
      "                \"printfBufferSize\": 3,\n"
      "                \"nativeKernelSupported\": "
      "\"12345678-1234-5678-1234-567812345678\"\n"
      "            },\n"
      "            \"ze_command_queue_group_properties_t\": [\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE\",\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS\"\n"
      "                    ],\n"
      "                    \"maxMemoryFillPatternSize\": 1,\n"
      "                    \"numQueues\": 2\n"
      "                },\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE\",\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS\"\n"
      "                    ],\n"
      "                    \"maxMemoryFillPatternSize\": 1,\n"
      "                    \"numQueues\": 2\n"
      "                }\n"
      "            ],\n"
      "            \"ze_scheduling_hint_exp_properties_t\": {\n"
      "                \"schedulingHintFlags\": [\n"
      "                    \"ZE_SCHEDULING_HINT_EXP_FLAG_ROUND_ROBIN\",\n"
      "                    "
      "\"ZE_SCHEDULING_HINT_EXP_FLAG_STALL_BASED_ROUND_ROBIN\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_float_atomic_ext_properties_t\": {\n"
      "                \"fp16Flags\": [\n"
      "                    "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_ADD\",\n"
      "                    "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\",\n"
      "                    \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX\"\n"
      "                ],\n"
      "                \"fp32Flags\": [\n"
      "                    "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\",\n"
      "                    \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX\"\n"
      "                ],\n"
      "                \"fp64Flags\": [\n"
      "                    \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_raytracing_ext_properties_t\": {\n"
      "                \"flags\": [\n"
      "                    \"ZE_DEVICE_RAYTRACING_EXT_FLAG_RAYQUERY\"\n"
      "                ],\n"
      "                \"maxBVHLevels\": 1\n"
      "            },\n"
      "            \"ze_device_memory_properties_t\": [\n"
      "                {\n"
      "                    \"name\": \"memory_name\",\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD\"\n"
      "                    ],\n"
      "                    \"maxClockRate\": 1,\n"
      "                    \"maxBusWidth\": 2,\n"
      "                    \"totalSize\": 3\n"
      "                },\n"
      "                {\n"
      "                    \"name\": \"memory_name\",\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD\"\n"
      "                    ],\n"
      "                    \"maxClockRate\": 1,\n"
      "                    \"maxBusWidth\": 2,\n"
      "                    \"totalSize\": 3\n"
      "                }\n"
      "            ],\n"
      "            \"ze_device_memory_access_properties_t\": {\n"
      "                \"hostAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\"\n"
      "                ],\n"
      "                \"deviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\"\n"
      "                ],\n"
      "                \"sharedSingleDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT_ATOMIC\"\n"
      "                ],\n"
      "                \"sharedCrossDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_RW\"\n"
      "                ],\n"
      "                \"sharedSystemAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_RW\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_cache_properties_t\": [\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL\"\n"
      "                    ],\n"
      "                    \"cacheSize\": 1\n"
      "                },\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL\"\n"
      "                    ],\n"
      "                    \"cacheSize\": 1\n"
      "                }\n"
      "            ],\n"
      "            \"ze_device_image_properties_t\": {\n"
      "                \"maxImageDims1D\": 1,\n"
      "                \"maxImageDims2D\": 2,\n"
      "                \"maxImageDims3D\": 3,\n"
      "                \"maxImageBufferSize\": 4,\n"
      "                \"maxImageArraySlices\": 5,\n"
      "                \"maxSamplers\": 6,\n"
      "                \"maxReadImageArgs\": 7,\n"
      "                \"maxWriteImageArgs\": 8\n"
      "            },\n"
      "            \"ze_device_external_memory_properties_t\": {\n"
      "                \"memoryAllocationImportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ],\n"
      "                \"memoryAllocationExportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ],\n"
      "                \"imageImportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ],\n"
      "                \"imageExportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ]\n"
      "            },\n"
      "            \"zet_device_debug_properties_t\": {\n"
      "                \"flags\": [\n"
      "                    \"ZET_DEVICE_DEBUG_PROPERTY_FLAG_ATTACH\"\n"
      "                ]\n"
      "            },\n"
      "            \"sub_devices_count\": 0,\n"
      "            \"sub_devices\": []\n"
      "        }\n"
      "    ]\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DriverPropertiesToJSON) {
  const auto properties = fake_driver_properties();

  const auto json = cs::driver_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected =
      "{\n"
      "    \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "    \"driverVersion\": 123\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DriverIpcPropertiesToJSON) {
  const auto properties = fake_driver_ipc_properties();

  const auto json = cs::driver_ipc_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected = "{\n"
                        "    \"flags\": [\n"
                        "        \"ZE_IPC_PROPERTY_FLAG_EVENT_POOL\",\n"
                        "        \"ZE_IPC_PROPERTY_FLAG_MEMORY\"\n"
                        "    ]\n"
                        "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, AllDriverExtensionPropertiesToJSON) {
  const auto properties = std::vector<ze_driver_extension_properties_t>{
      fake_driver_extension_properties(),
      ze_driver_extension_properties_t{"extension_b", ZE_API_VERSION_1_0}};

  const auto json = cs::all_driver_extension_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected = "[\n"
                        "        {\n"
                        "            \"name\": \"extension_a\",\n"
                        "            \"version\": \"1.0\"\n"
                        "        },\n"
                        "        {\n"
                        "            \"name\": \"extension_b\",\n"
                        "            \"version\": \"1.0\"\n"
                        "        }\n"
                        "]";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceCapabilitiesToJSON) {
  const auto capabilities = fake_device_with_sub_devices_capabilities();

  const auto json = cs::device_capabilities_to_json(capabilities);
  const auto actual = cs::ptree_to_string(json);
  const auto expected =
      "{\n"
      "    \"ze_device_properties_t\": {\n"
      "        \"name\": \"device_name\",\n"
      "        \"type\": \"ZE_DEVICE_TYPE_GPU\",\n"
      "        \"vendorId\": 1,\n"
      "        \"deviceId\": 2,\n"
      "        \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "        \"flags\": [\n"
      "            \"ZE_DEVICE_PROPERTY_FLAG_ECC\",\n"
      "            \"ZE_DEVICE_PROPERTY_FLAG_INTEGRATED\"\n"
      "        ],\n"
      "        \"coreClockRate\": 3,\n"
      "        \"maxMemAllocSize\": 4,\n"
      "        \"maxHardwareContexts\": 5,\n"
      "        \"maxCommandQueuePriority\": 6,\n"
      "        \"numThreadsPerEU\": 7,\n"
      "        \"physicalEUSimdWidth\": 8,\n"
      "        \"numEUsPerSubslice\": 9,\n"
      "        \"numSubslicesPerSlice\": 10,\n"
      "        \"numSlices\": 11,\n"
      "        \"timerResolution\": 12,\n"
      "        \"timestampValidBits\": 13,\n"
      "        \"kernelTimestampValidBits\": 14\n"
      "    },\n"
      "    \"ze_device_compute_properties_t\": {\n"
      "        \"maxTotalGroupSize\": 1,\n"
      "        \"maxGroupSizeX\": 2,\n"
      "        \"maxGroupSizeY\": 3,\n"
      "        \"maxGroupSizeZ\": 4,\n"
      "        \"maxGroupCountX\": 5,\n"
      "        \"maxGroupCountY\": 6,\n"
      "        \"maxGroupCountZ\": 7,\n"
      "        \"maxSharedLocalMemory\": 8,\n"
      "        \"subGroupSizes\": [\n"
      "            9,\n"
      "            10\n"
      "        ]\n"
      "    },\n"
      "    \"ze_device_module_properties_t\": {\n"
      "        \"spirvVersionSupported\": \"1.0\",\n"
      "        \"flags\": [\n"
      "            \"ZE_DEVICE_MODULE_FLAG_DP4A\",\n"
      "            \"ZE_DEVICE_MODULE_FLAG_FP16\"\n"
      "        ],\n"
      "        \"fp16flags\": [\n"
      "            \"ZE_DEVICE_FP_FLAG_DENORM\",\n"
      "            \"ZE_DEVICE_FP_FLAG_ROUND_TO_NEAREST\"\n"
      "        ],\n"
      "        \"fp32flags\": [\n"
      "            \"ZE_DEVICE_FP_FLAG_INF_NAN\",\n"
      "            \"ZE_DEVICE_FP_FLAG_ROUND_TO_ZERO\"\n"
      "        ],\n"
      "        \"fp64flags\": [\n"
      "            \"ZE_DEVICE_FP_FLAG_ROUNDED_DIVIDE_SQRT\",\n"
      "            \"ZE_DEVICE_FP_FLAG_ROUND_TO_INF\"\n"
      "        ],\n"
      "        \"maxArgumentsSize\": 2,\n"
      "        \"printfBufferSize\": 3,\n"
      "        \"nativeKernelSupported\": "
      "\"12345678-1234-5678-1234-567812345678\"\n"
      "    },\n"
      "    \"ze_command_queue_group_properties_t\": [\n"
      "        {\n"
      "            \"flags\": [\n"
      "                \"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE\",\n"
      "                "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS\"\n"
      "            ],\n"
      "            \"maxMemoryFillPatternSize\": 1,\n"
      "            \"numQueues\": 2\n"
      "        },\n"
      "        {\n"
      "            \"flags\": [\n"
      "                \"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE\",\n"
      "                "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS\"\n"
      "            ],\n"
      "            \"maxMemoryFillPatternSize\": 1,\n"
      "            \"numQueues\": 2\n"
      "        }\n"
      "    ],\n"
      "    \"ze_scheduling_hint_exp_properties_t\": {\n"
      "        \"schedulingHintFlags\": [\n"
      "            \"ZE_SCHEDULING_HINT_EXP_FLAG_ROUND_ROBIN\",\n"
      "            "
      "\"ZE_SCHEDULING_HINT_EXP_FLAG_STALL_BASED_ROUND_ROBIN\"\n"
      "        ]\n"
      "    },\n"
      "    \"ze_float_atomic_ext_properties_t\": {\n"
      "        \"fp16Flags\": [\n"
      "            "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_ADD\",\n"
      "            \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\",\n"
      "            \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX\"\n"
      "        ],\n"
      "        \"fp32Flags\": [\n"
      "            "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\",\n"
      "            \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX\"\n"
      "        ],\n"
      "        \"fp64Flags\": [\n"
      "            \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\"\n"
      "        ]\n"
      "    },\n"
      "    \"ze_device_raytracing_ext_properties_t\": {\n"
      "        \"flags\": [\n"
      "            \"ZE_DEVICE_RAYTRACING_EXT_FLAG_RAYQUERY\"\n"
      "        ],\n"
      "        \"maxBVHLevels\": 1\n"
      "    },\n"
      "    \"ze_device_memory_properties_t\": [\n"
      "        {\n"
      "            \"name\": \"memory_name\",\n"
      "            \"flags\": [\n"
      "                \"ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD\"\n"
      "            ],\n"
      "            \"maxClockRate\": 1,\n"
      "            \"maxBusWidth\": 2,\n"
      "            \"totalSize\": 3\n"
      "        },\n"
      "        {\n"
      "            \"name\": \"memory_name\",\n"
      "            \"flags\": [\n"
      "                \"ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD\"\n"
      "            ],\n"
      "            \"maxClockRate\": 1,\n"
      "            \"maxBusWidth\": 2,\n"
      "            \"totalSize\": 3\n"
      "        }\n"
      "    ],\n"
      "    \"ze_device_memory_access_properties_t\": {\n"
      "        \"hostAllocCapabilities\": [\n"
      "            \"ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC\",\n"
      "            \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\"\n"
      "        ],\n"
      "        \"deviceAllocCapabilities\": [\n"
      "            \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\"\n"
      "        ],\n"
      "        \"sharedSingleDeviceAllocCapabilities\": [\n"
      "            \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\",\n"
      "            \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT_ATOMIC\"\n"
      "        ],\n"
      "        \"sharedCrossDeviceAllocCapabilities\": [\n"
      "            \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\",\n"
      "            \"ZE_MEMORY_ACCESS_CAP_FLAG_RW\"\n"
      "        ],\n"
      "        \"sharedSystemAllocCapabilities\": [\n"
      "            \"ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC\",\n"
      "            \"ZE_MEMORY_ACCESS_CAP_FLAG_RW\"\n"
      "        ]\n"
      "    },\n"
      "    \"ze_device_cache_properties_t\": [\n"
      "        {\n"
      "            \"flags\": [\n"
      "                \"ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL\"\n"
      "            ],\n"
      "            \"cacheSize\": 1\n"
      "        },\n"
      "        {\n"
      "            \"flags\": [\n"
      "                \"ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL\"\n"
      "            ],\n"
      "            \"cacheSize\": 1\n"
      "        }\n"
      "    ],\n"
      "    \"ze_device_image_properties_t\": {\n"
      "        \"maxImageDims1D\": 1,\n"
      "        \"maxImageDims2D\": 2,\n"
      "        \"maxImageDims3D\": 3,\n"
      "        \"maxImageBufferSize\": 4,\n"
      "        \"maxImageArraySlices\": 5,\n"
      "        \"maxSamplers\": 6,\n"
      "        \"maxReadImageArgs\": 7,\n"
      "        \"maxWriteImageArgs\": 8\n"
      "    },\n"
      "    \"ze_device_external_memory_properties_t\": {\n"
      "        \"memoryAllocationImportTypes\": [\n"
      "            \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "            \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "        ],\n"
      "        \"memoryAllocationExportTypes\": [\n"
      "            \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "            \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "        ],\n"
      "        \"imageImportTypes\": [\n"
      "            \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "            \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "        ],\n"
      "        \"imageExportTypes\": [\n"
      "            \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "            \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "        ]\n"
      "    },\n"
      "    \"zet_device_debug_properties_t\": {\n"
      "        \"flags\": [\n"
      "            \"ZET_DEVICE_DEBUG_PROPERTY_FLAG_ATTACH\"\n"
      "        ]\n"
      "    },\n"
      "    \"sub_devices_count\": 2,\n"
      "    \"sub_devices\": [\n"
      "        {\n"
      "            \"ze_device_properties_t\": {\n"
      "                \"name\": \"device_name\",\n"
      "                \"type\": \"ZE_DEVICE_TYPE_GPU\",\n"
      "                \"vendorId\": 1,\n"
      "                \"deviceId\": 2,\n"
      "                \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "                \"flags\": [\n"
      "                    \"ZE_DEVICE_PROPERTY_FLAG_ECC\",\n"
      "                    \"ZE_DEVICE_PROPERTY_FLAG_INTEGRATED\",\n"
      "                    \"ZE_DEVICE_PROPERTY_FLAG_SUBDEVICE\"\n"
      "                ],\n"
      "                \"subdeviceId\": 15,\n"
      "                \"coreClockRate\": 3,\n"
      "                \"maxMemAllocSize\": 4,\n"
      "                \"maxHardwareContexts\": 5,\n"
      "                \"maxCommandQueuePriority\": 6,\n"
      "                \"numThreadsPerEU\": 7,\n"
      "                \"physicalEUSimdWidth\": 8,\n"
      "                \"numEUsPerSubslice\": 9,\n"
      "                \"numSubslicesPerSlice\": 10,\n"
      "                \"numSlices\": 11,\n"
      "                \"timerResolution\": 12,\n"
      "                \"timestampValidBits\": 13,\n"
      "                \"kernelTimestampValidBits\": 14\n"
      "            },\n"
      "            \"ze_device_compute_properties_t\": {\n"
      "                \"maxTotalGroupSize\": 1,\n"
      "                \"maxGroupSizeX\": 2,\n"
      "                \"maxGroupSizeY\": 3,\n"
      "                \"maxGroupSizeZ\": 4,\n"
      "                \"maxGroupCountX\": 5,\n"
      "                \"maxGroupCountY\": 6,\n"
      "                \"maxGroupCountZ\": 7,\n"
      "                \"maxSharedLocalMemory\": 8,\n"
      "                \"subGroupSizes\": [\n"
      "                    9,\n"
      "                    10\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_module_properties_t\": {\n"
      "                \"spirvVersionSupported\": \"1.0\",\n"
      "                \"flags\": [\n"
      "                    \"ZE_DEVICE_MODULE_FLAG_DP4A\",\n"
      "                    \"ZE_DEVICE_MODULE_FLAG_FP16\"\n"
      "                ],\n"
      "                \"fp16flags\": [\n"
      "                    \"ZE_DEVICE_FP_FLAG_DENORM\",\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUND_TO_NEAREST\"\n"
      "                ],\n"
      "                \"fp32flags\": [\n"
      "                    \"ZE_DEVICE_FP_FLAG_INF_NAN\",\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUND_TO_ZERO\"\n"
      "                ],\n"
      "                \"fp64flags\": [\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUNDED_DIVIDE_SQRT\",\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUND_TO_INF\"\n"
      "                ],\n"
      "                \"maxArgumentsSize\": 2,\n"
      "                \"printfBufferSize\": 3,\n"
      "                \"nativeKernelSupported\": "
      "\"12345678-1234-5678-1234-567812345678\"\n"
      "            },\n"
      "            \"ze_command_queue_group_properties_t\": [\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE\",\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS\"\n"
      "                    ],\n"
      "                    \"maxMemoryFillPatternSize\": 1,\n"
      "                    \"numQueues\": 2\n"
      "                },\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE\",\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS\"\n"
      "                    ],\n"
      "                    \"maxMemoryFillPatternSize\": 1,\n"
      "                    \"numQueues\": 2\n"
      "                }\n"
      "            ],\n"
      "            \"ze_scheduling_hint_exp_properties_t\": {\n"
      "                \"schedulingHintFlags\": [\n"
      "                    \"ZE_SCHEDULING_HINT_EXP_FLAG_ROUND_ROBIN\",\n"
      "                    "
      "\"ZE_SCHEDULING_HINT_EXP_FLAG_STALL_BASED_ROUND_ROBIN\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_float_atomic_ext_properties_t\": {\n"
      "                \"fp16Flags\": [\n"
      "                    "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_ADD\",\n"
      "                    "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\",\n"
      "                    \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX\"\n"
      "                ],\n"
      "                \"fp32Flags\": [\n"
      "                    "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\",\n"
      "                    \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX\"\n"
      "                ],\n"
      "                \"fp64Flags\": [\n"
      "                    \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_raytracing_ext_properties_t\": {\n"
      "                \"flags\": [\n"
      "                    \"ZE_DEVICE_RAYTRACING_EXT_FLAG_RAYQUERY\"\n"
      "                ],\n"
      "                \"maxBVHLevels\": 1\n"
      "            },\n"
      "            \"ze_device_memory_properties_t\": [\n"
      "                {\n"
      "                    \"name\": \"memory_name\",\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD\"\n"
      "                    ],\n"
      "                    \"maxClockRate\": 1,\n"
      "                    \"maxBusWidth\": 2,\n"
      "                    \"totalSize\": 3\n"
      "                },\n"
      "                {\n"
      "                    \"name\": \"memory_name\",\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD\"\n"
      "                    ],\n"
      "                    \"maxClockRate\": 1,\n"
      "                    \"maxBusWidth\": 2,\n"
      "                    \"totalSize\": 3\n"
      "                }\n"
      "            ],\n"
      "            \"ze_device_memory_access_properties_t\": {\n"
      "                \"hostAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\"\n"
      "                ],\n"
      "                \"deviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\"\n"
      "                ],\n"
      "                \"sharedSingleDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT_ATOMIC\"\n"
      "                ],\n"
      "                \"sharedCrossDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_RW\"\n"
      "                ],\n"
      "                \"sharedSystemAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_RW\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_cache_properties_t\": [\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL\"\n"
      "                    ],\n"
      "                    \"cacheSize\": 1\n"
      "                },\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL\"\n"
      "                    ],\n"
      "                    \"cacheSize\": 1\n"
      "                }\n"
      "            ],\n"
      "            \"ze_device_image_properties_t\": {\n"
      "                \"maxImageDims1D\": 1,\n"
      "                \"maxImageDims2D\": 2,\n"
      "                \"maxImageDims3D\": 3,\n"
      "                \"maxImageBufferSize\": 4,\n"
      "                \"maxImageArraySlices\": 5,\n"
      "                \"maxSamplers\": 6,\n"
      "                \"maxReadImageArgs\": 7,\n"
      "                \"maxWriteImageArgs\": 8\n"
      "            },\n"
      "            \"ze_device_external_memory_properties_t\": {\n"
      "                \"memoryAllocationImportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ],\n"
      "                \"memoryAllocationExportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ],\n"
      "                \"imageImportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ],\n"
      "                \"imageExportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ]\n"
      "            },\n"
      "            \"zet_device_debug_properties_t\": {\n"
      "                \"flags\": [\n"
      "                    \"ZET_DEVICE_DEBUG_PROPERTY_FLAG_ATTACH\"\n"
      "                ]\n"
      "            },\n"
      "            \"sub_devices_count\": 0,\n"
      "            \"sub_devices\": []\n"
      "        },\n"
      "        {\n"
      "            \"ze_device_properties_t\": {\n"
      "                \"name\": \"device_name\",\n"
      "                \"type\": \"ZE_DEVICE_TYPE_GPU\",\n"
      "                \"vendorId\": 1,\n"
      "                \"deviceId\": 2,\n"
      "                \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "                \"flags\": [\n"
      "                    \"ZE_DEVICE_PROPERTY_FLAG_ECC\",\n"
      "                    \"ZE_DEVICE_PROPERTY_FLAG_INTEGRATED\",\n"
      "                    \"ZE_DEVICE_PROPERTY_FLAG_SUBDEVICE\"\n"
      "                ],\n"
      "                \"subdeviceId\": 15,\n"
      "                \"coreClockRate\": 3,\n"
      "                \"maxMemAllocSize\": 4,\n"
      "                \"maxHardwareContexts\": 5,\n"
      "                \"maxCommandQueuePriority\": 6,\n"
      "                \"numThreadsPerEU\": 7,\n"
      "                \"physicalEUSimdWidth\": 8,\n"
      "                \"numEUsPerSubslice\": 9,\n"
      "                \"numSubslicesPerSlice\": 10,\n"
      "                \"numSlices\": 11,\n"
      "                \"timerResolution\": 12,\n"
      "                \"timestampValidBits\": 13,\n"
      "                \"kernelTimestampValidBits\": 14\n"
      "            },\n"
      "            \"ze_device_compute_properties_t\": {\n"
      "                \"maxTotalGroupSize\": 1,\n"
      "                \"maxGroupSizeX\": 2,\n"
      "                \"maxGroupSizeY\": 3,\n"
      "                \"maxGroupSizeZ\": 4,\n"
      "                \"maxGroupCountX\": 5,\n"
      "                \"maxGroupCountY\": 6,\n"
      "                \"maxGroupCountZ\": 7,\n"
      "                \"maxSharedLocalMemory\": 8,\n"
      "                \"subGroupSizes\": [\n"
      "                    9,\n"
      "                    10\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_module_properties_t\": {\n"
      "                \"spirvVersionSupported\": \"1.0\",\n"
      "                \"flags\": [\n"
      "                    \"ZE_DEVICE_MODULE_FLAG_DP4A\",\n"
      "                    \"ZE_DEVICE_MODULE_FLAG_FP16\"\n"
      "                ],\n"
      "                \"fp16flags\": [\n"
      "                    \"ZE_DEVICE_FP_FLAG_DENORM\",\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUND_TO_NEAREST\"\n"
      "                ],\n"
      "                \"fp32flags\": [\n"
      "                    \"ZE_DEVICE_FP_FLAG_INF_NAN\",\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUND_TO_ZERO\"\n"
      "                ],\n"
      "                \"fp64flags\": [\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUNDED_DIVIDE_SQRT\",\n"
      "                    \"ZE_DEVICE_FP_FLAG_ROUND_TO_INF\"\n"
      "                ],\n"
      "                \"maxArgumentsSize\": 2,\n"
      "                \"printfBufferSize\": 3,\n"
      "                \"nativeKernelSupported\": "
      "\"12345678-1234-5678-1234-567812345678\"\n"
      "            },\n"
      "            \"ze_command_queue_group_properties_t\": [\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE\",\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS\"\n"
      "                    ],\n"
      "                    \"maxMemoryFillPatternSize\": 1,\n"
      "                    \"numQueues\": 2\n"
      "                },\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE\",\n"
      "                        "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS\"\n"
      "                    ],\n"
      "                    \"maxMemoryFillPatternSize\": 1,\n"
      "                    \"numQueues\": 2\n"
      "                }\n"
      "            ],\n"
      "            \"ze_scheduling_hint_exp_properties_t\": {\n"
      "                \"schedulingHintFlags\": [\n"
      "                    \"ZE_SCHEDULING_HINT_EXP_FLAG_ROUND_ROBIN\",\n"
      "                    "
      "\"ZE_SCHEDULING_HINT_EXP_FLAG_STALL_BASED_ROUND_ROBIN\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_float_atomic_ext_properties_t\": {\n"
      "                \"fp16Flags\": [\n"
      "                    "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_ADD\",\n"
      "                    "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\",\n"
      "                    \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX\"\n"
      "                ],\n"
      "                \"fp32Flags\": [\n"
      "                    "
      "\"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\",\n"
      "                    \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_MIN_MAX\"\n"
      "                ],\n"
      "                \"fp64Flags\": [\n"
      "                    \"ZE_DEVICE_FP_ATOMIC_EXT_FLAG_GLOBAL_LOAD_STORE\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_raytracing_ext_properties_t\": {\n"
      "                \"flags\": [\n"
      "                    \"ZE_DEVICE_RAYTRACING_EXT_FLAG_RAYQUERY\"\n"
      "                ],\n"
      "                \"maxBVHLevels\": 1\n"
      "            },\n"
      "            \"ze_device_memory_properties_t\": [\n"
      "                {\n"
      "                    \"name\": \"memory_name\",\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD\"\n"
      "                    ],\n"
      "                    \"maxClockRate\": 1,\n"
      "                    \"maxBusWidth\": 2,\n"
      "                    \"totalSize\": 3\n"
      "                },\n"
      "                {\n"
      "                    \"name\": \"memory_name\",\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD\"\n"
      "                    ],\n"
      "                    \"maxClockRate\": 1,\n"
      "                    \"maxBusWidth\": 2,\n"
      "                    \"totalSize\": 3\n"
      "                }\n"
      "            ],\n"
      "            \"ze_device_memory_access_properties_t\": {\n"
      "                \"hostAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\"\n"
      "                ],\n"
      "                \"deviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\"\n"
      "                ],\n"
      "                \"sharedSingleDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT_ATOMIC\"\n"
      "                ],\n"
      "                \"sharedCrossDeviceAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_RW\"\n"
      "                ],\n"
      "                \"sharedSystemAllocCapabilities\": [\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC\",\n"
      "                    \"ZE_MEMORY_ACCESS_CAP_FLAG_RW\"\n"
      "                ]\n"
      "            },\n"
      "            \"ze_device_cache_properties_t\": [\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL\"\n"
      "                    ],\n"
      "                    \"cacheSize\": 1\n"
      "                },\n"
      "                {\n"
      "                    \"flags\": [\n"
      "                        \"ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL\"\n"
      "                    ],\n"
      "                    \"cacheSize\": 1\n"
      "                }\n"
      "            ],\n"
      "            \"ze_device_image_properties_t\": {\n"
      "                \"maxImageDims1D\": 1,\n"
      "                \"maxImageDims2D\": 2,\n"
      "                \"maxImageDims3D\": 3,\n"
      "                \"maxImageBufferSize\": 4,\n"
      "                \"maxImageArraySlices\": 5,\n"
      "                \"maxSamplers\": 6,\n"
      "                \"maxReadImageArgs\": 7,\n"
      "                \"maxWriteImageArgs\": 8\n"
      "            },\n"
      "            \"ze_device_external_memory_properties_t\": {\n"
      "                \"memoryAllocationImportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ],\n"
      "                \"memoryAllocationExportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ],\n"
      "                \"imageImportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ],\n"
      "                \"imageExportTypes\": [\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
      "                    \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
      "                ]\n"
      "            },\n"
      "            \"zet_device_debug_properties_t\": {\n"
      "                \"flags\": [\n"
      "                    \"ZET_DEVICE_DEBUG_PROPERTY_FLAG_ATTACH\"\n"
      "                ]\n"
      "            },\n"
      "            \"sub_devices_count\": 0,\n"
      "            \"sub_devices\": []\n"
      "        }\n"
      "    ]\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DevicePropertiesToJSON) {
  const auto properties = fake_device_properties();

  const auto json = cs::device_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected =
      "{\n"
      "    \"name\": \"device_name\",\n"
      "    \"type\": \"ZE_DEVICE_TYPE_GPU\",\n"
      "    \"vendorId\": 1,\n"
      "    \"deviceId\": 2,\n"
      "    \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "    \"flags\": [\n"
      "        \"ZE_DEVICE_PROPERTY_FLAG_ECC\",\n"
      "        \"ZE_DEVICE_PROPERTY_FLAG_INTEGRATED\"\n"
      "    ],\n"
      "    \"coreClockRate\": 3,\n"
      "    \"maxMemAllocSize\": 4,\n"
      "    \"maxHardwareContexts\": 5,\n"
      "    \"maxCommandQueuePriority\": 6,\n"
      "    \"numThreadsPerEU\": 7,\n"
      "    \"physicalEUSimdWidth\": 8,\n"
      "    \"numEUsPerSubslice\": 9,\n"
      "    \"numSubslicesPerSlice\": 10,\n"
      "    \"numSlices\": 11,\n"
      "    \"timerResolution\": 12,\n"
      "    \"timestampValidBits\": 13,\n"
      "    \"kernelTimestampValidBits\": 14\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, SubDevicePropertiesToJSON) {
  const auto properties = fake_sub_device_properties();

  const auto json = cs::device_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected =
      "{\n"
      "    \"name\": \"device_name\",\n"
      "    \"type\": \"ZE_DEVICE_TYPE_GPU\",\n"
      "    \"vendorId\": 1,\n"
      "    \"deviceId\": 2,\n"
      "    \"uuid\": \"12345678-1234-5678-1234-567812345678\",\n"
      "    \"flags\": [\n"
      "        \"ZE_DEVICE_PROPERTY_FLAG_ECC\",\n"
      "        \"ZE_DEVICE_PROPERTY_FLAG_INTEGRATED\",\n"
      "        \"ZE_DEVICE_PROPERTY_FLAG_SUBDEVICE\"\n"
      "    ],\n"
      "    \"subdeviceId\": 15,\n"
      "    \"coreClockRate\": 3,\n"
      "    \"maxMemAllocSize\": 4,\n"
      "    \"maxHardwareContexts\": 5,\n"
      "    \"maxCommandQueuePriority\": 6,\n"
      "    \"numThreadsPerEU\": 7,\n"
      "    \"physicalEUSimdWidth\": 8,\n"
      "    \"numEUsPerSubslice\": 9,\n"
      "    \"numSubslicesPerSlice\": 10,\n"
      "    \"numSlices\": 11,\n"
      "    \"timerResolution\": 12,\n"
      "    \"timestampValidBits\": 13,\n"
      "    \"kernelTimestampValidBits\": 14\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceComputePropertiesToJSON) {
  const auto properties = fake_device_compute_properties();

  const auto json = cs::device_compute_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected = "{\n"
                        "    \"maxTotalGroupSize\": 1,\n"
                        "    \"maxGroupSizeX\": 2,\n"
                        "    \"maxGroupSizeY\": 3,\n"
                        "    \"maxGroupSizeZ\": 4,\n"
                        "    \"maxGroupCountX\": 5,\n"
                        "    \"maxGroupCountY\": 6,\n"
                        "    \"maxGroupCountZ\": 7,\n"
                        "    \"maxSharedLocalMemory\": 8,\n"
                        "    \"subGroupSizes\": [\n"
                        "        9,\n"
                        "        10\n"
                        "    ]\n"
                        "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceModulePropertiesToJSON) {
  const auto properties = fake_device_module_properties();

  const auto json = cs::device_module_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected = "{\n"
                        "    \"spirvVersionSupported\": \"1.0\",\n"
                        "    \"flags\": [\n"
                        "        \"ZE_DEVICE_MODULE_FLAG_DP4A\",\n"
                        "        \"ZE_DEVICE_MODULE_FLAG_FP16\"\n"
                        "    ],\n"
                        "    \"fp16flags\": [\n"
                        "        \"ZE_DEVICE_FP_FLAG_DENORM\",\n"
                        "        \"ZE_DEVICE_FP_FLAG_ROUND_TO_NEAREST\"\n"
                        "    ],\n"
                        "    \"fp32flags\": [\n"
                        "        \"ZE_DEVICE_FP_FLAG_INF_NAN\",\n"
                        "        \"ZE_DEVICE_FP_FLAG_ROUND_TO_ZERO\"\n"
                        "    ],\n"
                        "    \"fp64flags\": [\n"
                        "        \"ZE_DEVICE_FP_FLAG_ROUNDED_DIVIDE_SQRT\",\n"
                        "        \"ZE_DEVICE_FP_FLAG_ROUND_TO_INF\"\n"
                        "    ],\n"
                        "    \"maxArgumentsSize\": 2,\n"
                        "    \"printfBufferSize\": 3,\n"
                        "    \"nativeKernelSupported\": "
                        "\"12345678-1234-5678-1234-567812345678\"\n"
                        "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, AllDeviceCommandQueueGroupPropertiesToJSON) {
  const auto properties = std::vector<ze_command_queue_group_properties_t>{
      fake_device_command_queue_group_properties(),
      fake_device_command_queue_group_properties()};

  const auto json =
      cs::all_device_command_queue_group_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected =
      "[\n"
      "        {\n"
      "            \"flags\": [\n"
      "                \"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE\",\n"
      "                "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS\"\n"
      "            ],\n"
      "            \"maxMemoryFillPatternSize\": 1,\n"
      "            \"numQueues\": 2\n"
      "        },\n"
      "        {\n"
      "            \"flags\": [\n"
      "                \"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE\",\n"
      "                "
      "\"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS\"\n"
      "            ],\n"
      "            \"maxMemoryFillPatternSize\": 1,\n"
      "            \"numQueues\": 2\n"
      "        }\n"
      "]";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceCommandQueueGroupPropertiesToJSON) {
  const auto properties = fake_device_command_queue_group_properties();

  const auto json =
      cs::device_command_queue_group_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected =
      "{\n"
      "    \"flags\": [\n"
      "        \"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COMPUTE\",\n"
      "        \"ZE_COMMAND_QUEUE_GROUP_PROPERTY_FLAG_COOPERATIVE_KERNELS\"\n"
      "    ],\n"
      "    \"maxMemoryFillPatternSize\": 1,\n"
      "    \"numQueues\": 2\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, AllDeviceMemoryPropertiesToJSON) {
  const auto properties = std::vector<ze_device_memory_properties_t>{
      fake_device_memory_properties(), fake_device_memory_properties()};

  const auto json = cs::all_device_memory_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected =
      "[\n"
      "        {\n"
      "            \"name\": \"memory_name\",\n"
      "            \"flags\": [\n"
      "                \"ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD\"\n"
      "            ],\n"
      "            \"maxClockRate\": 1,\n"
      "            \"maxBusWidth\": 2,\n"
      "            \"totalSize\": 3\n"
      "        },\n"
      "        {\n"
      "            \"name\": \"memory_name\",\n"
      "            \"flags\": [\n"
      "                \"ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD\"\n"
      "            ],\n"
      "            \"maxClockRate\": 1,\n"
      "            \"maxBusWidth\": 2,\n"
      "            \"totalSize\": 3\n"
      "        }\n"
      "]";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceMemoryPropertiesToJSON) {
  const auto properties = fake_device_memory_properties();

  const auto json = cs::device_memory_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected = "{\n"
                        "    \"name\": \"memory_name\",\n"
                        "    \"flags\": [\n"
                        "        \"ZE_DEVICE_MEMORY_PROPERTY_FLAG_TBD\"\n"
                        "    ],\n"
                        "    \"maxClockRate\": 1,\n"
                        "    \"maxBusWidth\": 2,\n"
                        "    \"totalSize\": 3\n"
                        "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceMemoryAccessPropertiesToJSON) {
  const auto properties = fake_device_memory_access_properties();

  const auto json = cs::device_memory_access_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected =
      "{\n"
      "    \"hostAllocCapabilities\": [\n"
      "        \"ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC\",\n"
      "        \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\"\n"
      "    ],\n"
      "    \"deviceAllocCapabilities\": [\n"
      "        \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\"\n"
      "    ],\n"
      "    \"sharedSingleDeviceAllocCapabilities\": [\n"
      "        \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\",\n"
      "        \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT_ATOMIC\"\n"
      "    ],\n"
      "    \"sharedCrossDeviceAllocCapabilities\": [\n"
      "        \"ZE_MEMORY_ACCESS_CAP_FLAG_CONCURRENT\",\n"
      "        \"ZE_MEMORY_ACCESS_CAP_FLAG_RW\"\n"
      "    ],\n"
      "    \"sharedSystemAllocCapabilities\": [\n"
      "        \"ZE_MEMORY_ACCESS_CAP_FLAG_ATOMIC\",\n"
      "        \"ZE_MEMORY_ACCESS_CAP_FLAG_RW\"\n"
      "    ]\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, AllDeviceCachePropertiesToJSON) {
  const auto properties = std::vector<ze_device_cache_properties_t>{
      fake_device_cache_properties(), fake_device_cache_properties()};

  const auto json = cs::all_device_cache_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected =
      "[\n"
      "        {\n"
      "            \"flags\": [\n"
      "                \"ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL\"\n"
      "            ],\n"
      "            \"cacheSize\": 1\n"
      "        },\n"
      "        {\n"
      "            \"flags\": [\n"
      "                \"ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL\"\n"
      "            ],\n"
      "            \"cacheSize\": 1\n"
      "        }\n"
      "]";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceCachePropertiesToJSON) {
  const auto properties = fake_device_cache_properties();

  const auto json = cs::device_cache_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected =
      "{\n"
      "    \"flags\": [\n"
      "        \"ZE_DEVICE_CACHE_PROPERTY_FLAG_USER_CONTROL\"\n"
      "    ],\n"
      "    \"cacheSize\": 1\n"
      "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceImagePropertiesToJSON) {
  const auto properties = fake_device_image_properties();

  const auto json = cs::device_image_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected = "{\n"
                        "    \"maxImageDims1D\": 1,\n"
                        "    \"maxImageDims2D\": 2,\n"
                        "    \"maxImageDims3D\": 3,\n"
                        "    \"maxImageBufferSize\": 4,\n"
                        "    \"maxImageArraySlices\": 5,\n"
                        "    \"maxSamplers\": 6,\n"
                        "    \"maxReadImageArgs\": 7,\n"
                        "    \"maxWriteImageArgs\": 8\n"
                        "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceExternalMemoryPropertiesToJSON) {
  const auto properties = fake_device_external_memory_properties();

  const auto json = cs::device_external_memory_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected = "{\n"
                        "    \"memoryAllocationImportTypes\": [\n"
                        "        \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
                        "        \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
                        "    ],\n"
                        "    \"memoryAllocationExportTypes\": [\n"
                        "        \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
                        "        \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
                        "    ],\n"
                        "    \"imageImportTypes\": [\n"
                        "        \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
                        "        \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
                        "    ],\n"
                        "    \"imageExportTypes\": [\n"
                        "        \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_DMA_BUF\",\n"
                        "        \"ZE_EXTERNAL_MEMORY_TYPE_FLAG_OPAQUE_FD\"\n"
                        "    ]\n"
                        "}";

  EXPECT_THAT(actual, ::testing::StrEq(expected));
}

TEST(JSONFormatterTests, DeviceDebugPropertiesToJSON) {
  const auto properties = fake_device_debug_properties();

  const auto json = cs::device_debug_properties_to_json(properties);
  const auto actual = cs::ptree_to_string(json);
  const auto expected = "{\n"
                        "    \"flags\": [\n"
                        "        \"ZET_DEVICE_DEBUG_PROPERTY_FLAG_ATTACH\"\n"
                        "    ]\n"
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
  const std::string expected = "{\n    \"value\": \"1.5\"\n}";

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
