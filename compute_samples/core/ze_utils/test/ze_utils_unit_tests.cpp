/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "ze_utils/ze_utils.hpp"
#include "gtest/gtest.h"

TEST(ZeApiVersionToString, ZE_API_VERSION_1_0) {
  const ze_api_version_t v = ZE_API_VERSION_1_0;
  EXPECT_EQ("0.91", compute_samples::to_string(v));
}

TEST(ZeResultToString, ZE_RESULT_SUCCESS) {
  const ze_result_t r = ZE_RESULT_SUCCESS;
  EXPECT_EQ("ZE_RESULT_SUCCESS", compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_NOT_READY) {
  const ze_result_t r = ZE_RESULT_NOT_READY;
  EXPECT_EQ("ZE_RESULT_NOT_READY", compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_DEVICE_LOST) {
  const ze_result_t r = ZE_RESULT_ERROR_DEVICE_LOST;
  EXPECT_EQ("ZE_RESULT_ERROR_DEVICE_LOST", compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY) {
  const ze_result_t r = ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY;
  EXPECT_EQ("ZE_RESULT_ERROR_OUT_OF_HOST_MEMORY",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_OUT_OF_DEVICE_MEMORY) {
  const ze_result_t r = ZE_RESULT_ERROR_OUT_OF_DEVICE_MEMORY;
  EXPECT_EQ("ZE_RESULT_ERROR_OUT_OF_DEVICE_MEMORY",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_MODULE_BUILD_FAILURE) {
  const ze_result_t r = ZE_RESULT_ERROR_MODULE_BUILD_FAILURE;
  EXPECT_EQ("ZE_RESULT_ERROR_MODULE_BUILD_FAILURE",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INSUFFICIENT_PERMISSIONS) {
  const ze_result_t r = ZE_RESULT_ERROR_INSUFFICIENT_PERMISSIONS;
  EXPECT_EQ("ZE_RESULT_ERROR_INSUFFICIENT_PERMISSIONS",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_NOT_AVAILABLE) {
  const ze_result_t r = ZE_RESULT_ERROR_NOT_AVAILABLE;
  EXPECT_EQ("ZE_RESULT_ERROR_NOT_AVAILABLE", compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_UNINITIALIZED) {
  const ze_result_t r = ZE_RESULT_ERROR_UNINITIALIZED;
  EXPECT_EQ("ZE_RESULT_ERROR_UNINITIALIZED", compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_UNSUPPORTED_VERSION) {
  const ze_result_t r = ZE_RESULT_ERROR_UNSUPPORTED_VERSION;
  EXPECT_EQ("ZE_RESULT_ERROR_UNSUPPORTED_VERSION",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_UNSUPPORTED_FEATURE) {
  const ze_result_t r = ZE_RESULT_ERROR_UNSUPPORTED_FEATURE;
  EXPECT_EQ("ZE_RESULT_ERROR_UNSUPPORTED_FEATURE",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_ARGUMENT) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_ARGUMENT;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_ARGUMENT", compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_NULL_HANDLE) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_NULL_HANDLE;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_NULL_HANDLE",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_HANDLE_OBJECT_IN_USE) {
  const ze_result_t r = ZE_RESULT_ERROR_HANDLE_OBJECT_IN_USE;
  EXPECT_EQ("ZE_RESULT_ERROR_HANDLE_OBJECT_IN_USE",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_NULL_POINTER) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_NULL_POINTER;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_NULL_POINTER",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_SIZE) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_SIZE;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_SIZE", compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_UNSUPPORTED_SIZE) {
  const ze_result_t r = ZE_RESULT_ERROR_UNSUPPORTED_SIZE;
  EXPECT_EQ("ZE_RESULT_ERROR_UNSUPPORTED_SIZE", compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_UNSUPPORTED_ALIGNMENT) {
  const ze_result_t r = ZE_RESULT_ERROR_UNSUPPORTED_ALIGNMENT;
  EXPECT_EQ("ZE_RESULT_ERROR_UNSUPPORTED_ALIGNMENT",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_SYNCHRONIZATION_OBJECT) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_SYNCHRONIZATION_OBJECT;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_SYNCHRONIZATION_OBJECT",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_ENUMERATION) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_ENUMERATION;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_ENUMERATION",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_UNSUPPORTED_ENUMERATION) {
  const ze_result_t r = ZE_RESULT_ERROR_UNSUPPORTED_ENUMERATION;
  EXPECT_EQ("ZE_RESULT_ERROR_UNSUPPORTED_ENUMERATION",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_UNSUPPORTED_IMAGE_FORMAT) {
  const ze_result_t r = ZE_RESULT_ERROR_UNSUPPORTED_IMAGE_FORMAT;
  EXPECT_EQ("ZE_RESULT_ERROR_UNSUPPORTED_IMAGE_FORMAT",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_NATIVE_BINARY) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_NATIVE_BINARY;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_NATIVE_BINARY",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_GLOBAL_NAME) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_GLOBAL_NAME;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_GLOBAL_NAME",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_KERNEL_NAME) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_KERNEL_NAME;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_KERNEL_NAME",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_FUNCTION_NAME) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_FUNCTION_NAME;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_FUNCTION_NAME",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_GROUP_SIZE_DIMENSION) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_GROUP_SIZE_DIMENSION;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_GROUP_SIZE_DIMENSION",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_GLOBAL_WIDTH_DIMENSION) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_GLOBAL_WIDTH_DIMENSION;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_GLOBAL_WIDTH_DIMENSION",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_INDEX) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_INDEX;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_INDEX",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_SIZE) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_SIZE;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_KERNEL_ARGUMENT_SIZE",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_KERNEL_ATTRIBUTE_VALUE) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_KERNEL_ATTRIBUTE_VALUE;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_KERNEL_ATTRIBUTE_VALUE",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_INVALID_COMMAND_LIST_TYPE) {
  const ze_result_t r = ZE_RESULT_ERROR_INVALID_COMMAND_LIST_TYPE;
  EXPECT_EQ("ZE_RESULT_ERROR_INVALID_COMMAND_LIST_TYPE",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_OVERLAPPING_REGIONS) {
  const ze_result_t r = ZE_RESULT_ERROR_OVERLAPPING_REGIONS;
  EXPECT_EQ("ZE_RESULT_ERROR_OVERLAPPING_REGIONS",
            compute_samples::to_string(r));
}

TEST(ZeResultToString, ZE_RESULT_ERROR_UNKNOWN) {
  const ze_result_t r = ZE_RESULT_ERROR_UNKNOWN;
  EXPECT_EQ("ZE_RESULT_ERROR_UNKNOWN", compute_samples::to_string(r));
}

TEST(ZeResultToString, InvalidValue) {
  const int invalid_value = 100;
  const auto r = static_cast<ze_result_t>(invalid_value);
  EXPECT_EQ("Unknown ze_result_t value: " + std::to_string(100),
            compute_samples::to_string(r));
}

TEST(ZeCommandQueueDescVersionToString, ZE_COMMAND_QUEUE_DESC_VERSION_CURRENT) {
  const ze_command_queue_desc_version_t d =
      ZE_COMMAND_QUEUE_DESC_VERSION_CURRENT;
  EXPECT_EQ("ZE_COMMAND_QUEUE_DESC_VERSION_CURRENT",
            compute_samples::to_string(d));
}

TEST(ZeCommandQueueDescVersionToString, InvalidValue) {
  const int invalid_value = ZE_COMMAND_QUEUE_DESC_VERSION_CURRENT + 1;
  const auto d = static_cast<ze_command_queue_desc_version_t>(
      static_cast<uint32_t>(invalid_value));
  EXPECT_EQ("Unknown ze_command_queue_desc_version_t value: " +
                std::to_string(invalid_value),
            compute_samples::to_string(d));
}

TEST(ZeCommandQueueDescFlagsToString, ZE_COMMAND_QUEUE_FLAG_COPY_ONLY) {
  const ze_command_queue_flag_t d = ZE_COMMAND_QUEUE_FLAG_COPY_ONLY;
  EXPECT_EQ("ZE_COMMAND_QUEUE_FLAG_COPY_ONLY", compute_samples::to_string(d));
}

TEST(ZeCommandQueueDescFlagsToString, InvalidValue) {
  const int invalid_value =
      ZE_COMMAND_QUEUE_FLAG_COPY_ONLY + ZE_COMMAND_QUEUE_FLAG_LOGICAL_ONLY;
  const auto d = static_cast<ze_command_queue_flag_t>(
      static_cast<uint32_t>(invalid_value));
  EXPECT_EQ("Unknown ze_command_queue_flag_t value: " +
                std::to_string(invalid_value),
            compute_samples::to_string(d));
}

TEST(ZeCommandQueueDescModeToString, ZE_COMMAND_QUEUE_MODE_SYNCHRONOUS) {
  const ze_command_queue_mode_t d = ZE_COMMAND_QUEUE_MODE_SYNCHRONOUS;
  EXPECT_EQ("ZE_COMMAND_QUEUE_MODE_SYNCHRONOUS", compute_samples::to_string(d));
}

TEST(ZeCommandQueueDescModeToString, InvalidValue) {
  const int invalid_value =
      ZE_COMMAND_QUEUE_MODE_SYNCHRONOUS + ZE_COMMAND_QUEUE_MODE_ASYNCHRONOUS;
  const auto d = static_cast<ze_command_queue_mode_t>(
      static_cast<uint32_t>(invalid_value));
  EXPECT_EQ("Unknown ze_command_queue_mode_t value: " +
                std::to_string(invalid_value),
            compute_samples::to_string(d));
}

TEST(ZeCommandQueueDescPriorityToString, ZE_COMMAND_QUEUE_PRIORITY_LOW) {
  const ze_command_queue_priority_t d = ZE_COMMAND_QUEUE_PRIORITY_LOW;
  EXPECT_EQ("ZE_COMMAND_QUEUE_PRIORITY_LOW", compute_samples::to_string(d));
}

TEST(ZeCommandQueueDescPriorityToString, InvalidValue) {
  const int invalid_value =
      ZE_COMMAND_QUEUE_PRIORITY_LOW + ZE_COMMAND_QUEUE_PRIORITY_HIGH;
  const auto d = static_cast<ze_command_queue_priority_t>(
      static_cast<uint32_t>(invalid_value));
  EXPECT_EQ("Unknown ze_command_queue_priority_t value: " +
                std::to_string(invalid_value),
            compute_samples::to_string(d));
}

TEST(ZeImageDescVersionToString, ZE_IMAGE_DESC_VERSION_CURRENT) {
  const ze_image_desc_version_t v = ZE_IMAGE_DESC_VERSION_CURRENT;
  EXPECT_EQ("ZE_IMAGE_DESC_VERSION_CURRENT", compute_samples::to_string(v));
}

TEST(ZeImageDescVersionToString, InvalidValue) {
  const int invalid_value = ZE_IMAGE_DESC_VERSION_CURRENT + 1;
  const auto d = static_cast<ze_image_desc_version_t>(
      static_cast<uint32_t>(invalid_value));
  EXPECT_EQ("Unknown ze_image_desc_version_t value: " +
                std::to_string(invalid_value),
            compute_samples::to_string(d));
}

TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_8) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_8;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_8", compute_samples::to_string(l));
}

TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_16) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_16;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_16", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_32) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_32;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_32", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_8_8) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_8_8;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_8_8", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_8_8_8_8) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_8_8_8_8;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_8_8_8_8", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_16_16) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_16_16;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_16_16", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_16_16_16_16) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_16_16_16_16;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_16_16_16_16",
            compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_32_32) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_32_32;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_32_32", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_32_32_32_32) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_32_32_32_32;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_32_32_32_32",
            compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_10_10_10_2) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_10_10_10_2;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_10_10_10_2", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_11_11_10) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_11_11_10;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_11_11_10", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_5_6_5) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_5_6_5;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_5_6_5", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_5_5_5_1) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_5_5_5_1;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_5_5_5_1", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_4_4_4_4) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_4_4_4_4;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_4_4_4_4", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_Y8) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_Y8;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_Y8", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_NV12) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_NV12;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_NV12", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_YUYV) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_YUYV;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_YUYV", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_VYUY) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_VYUY;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_VYUY", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_YVYU) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_YVYU;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_YVYU", compute_samples::to_string(l));
}
TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_UYVY) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_UYVY;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_UYVY", compute_samples::to_string(l));
}

TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_AYUV) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_AYUV;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_AYUV", compute_samples::to_string(l));
}

TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_YUAV) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_YUAV;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_YUAV", compute_samples::to_string(l));
}

TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_P010) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_P010;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_P010", compute_samples::to_string(l));
}

TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_Y410) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_Y410;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_Y410", compute_samples::to_string(l));
}

TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_P012) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_P012;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_P012", compute_samples::to_string(l));
}

TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_Y16) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_Y16;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_Y16", compute_samples::to_string(l));
}

TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_P016) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_P016;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_P016", compute_samples::to_string(l));
}

TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_Y216) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_Y216;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_Y216", compute_samples::to_string(l));
}

TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_P216) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_P216;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_P216", compute_samples::to_string(l));
}

TEST(ZeImageFormatLayoutToString, ZE_IMAGE_FORMAT_LAYOUT_P416) {
  const ze_image_format_layout_t l = ZE_IMAGE_FORMAT_LAYOUT_P416;
  EXPECT_EQ("ZE_IMAGE_FORMAT_LAYOUT_P416", compute_samples::to_string(l));
}

TEST(ZeImageFormatLayoutToString, InvalidValue) {
  const int invalid_value = ZE_IMAGE_FORMAT_LAYOUT_P416 + 1;
  const auto l = static_cast<ze_image_format_layout_t>(
      static_cast<uint32_t>(invalid_value));
  EXPECT_EQ("Unknown ze_image_format_layout_t value: " +
                std::to_string(invalid_value),
            compute_samples::to_string(l));
}

TEST(ZeImageFormatTypeToString, ZE_IMAGE_FORMAT_TYPE_UINT) {
  const ze_image_format_type_t t = ZE_IMAGE_FORMAT_TYPE_UINT;
  EXPECT_EQ("ZE_IMAGE_FORMAT_TYPE_UINT", compute_samples::to_string(t));
}

TEST(ZeImageFormatTypeToString, ZE_IMAGE_FORMAT_TYPE_SINT) {
  const ze_image_format_type_t t = ZE_IMAGE_FORMAT_TYPE_SINT;
  EXPECT_EQ("ZE_IMAGE_FORMAT_TYPE_SINT", compute_samples::to_string(t));
}

TEST(ZeImageFormatTypeToString, ZE_IMAGE_FORMAT_TYPE_UNORM) {
  const ze_image_format_type_t t = ZE_IMAGE_FORMAT_TYPE_UNORM;
  EXPECT_EQ("ZE_IMAGE_FORMAT_TYPE_UNORM", compute_samples::to_string(t));
}

TEST(ZeImageFormatTypeToString, ZE_IMAGE_FORMAT_TYPE_SNORM) {
  const ze_image_format_type_t t = ZE_IMAGE_FORMAT_TYPE_SNORM;
  EXPECT_EQ("ZE_IMAGE_FORMAT_TYPE_SNORM", compute_samples::to_string(t));
}

TEST(ZeImageFormatTypeToString, ZE_IMAGE_FORMAT_TYPE_FLOAT) {
  const ze_image_format_type_t t = ZE_IMAGE_FORMAT_TYPE_FLOAT;
  EXPECT_EQ("ZE_IMAGE_FORMAT_TYPE_FLOAT", compute_samples::to_string(t));
}

TEST(ZeImageFormatTypeToString, InvalidValue) {
  const int invalid_value = ZE_IMAGE_FORMAT_TYPE_FLOAT + 1;
  const auto t =
      static_cast<ze_image_format_type_t>(static_cast<uint32_t>(invalid_value));
  EXPECT_EQ("Unknown ze_image_format_type_t value: " +
                std::to_string(invalid_value),
            compute_samples::to_string(t));
}

TEST(ZeImageFormatSwizzleToString, ZE_IMAGE_FORMAT_SWIZZLE_R) {
  const ze_image_format_swizzle_t t = ZE_IMAGE_FORMAT_SWIZZLE_R;
  EXPECT_EQ("ZE_IMAGE_FORMAT_SWIZZLE_R", compute_samples::to_string(t));
}

TEST(ZeImageFormatSwizzleToString, ZE_IMAGE_FORMAT_SWIZZLE_G) {
  const ze_image_format_swizzle_t t = ZE_IMAGE_FORMAT_SWIZZLE_G;
  EXPECT_EQ("ZE_IMAGE_FORMAT_SWIZZLE_G", compute_samples::to_string(t));
}

TEST(ZeImageFormatSwizzleToString, ZE_IMAGE_FORMAT_SWIZZLE_B) {
  const ze_image_format_swizzle_t t = ZE_IMAGE_FORMAT_SWIZZLE_B;
  EXPECT_EQ("ZE_IMAGE_FORMAT_SWIZZLE_B", compute_samples::to_string(t));
}

TEST(ZeImageFormatSwizzleToString, ZE_IMAGE_FORMAT_SWIZZLE_A) {
  const ze_image_format_swizzle_t t = ZE_IMAGE_FORMAT_SWIZZLE_A;
  EXPECT_EQ("ZE_IMAGE_FORMAT_SWIZZLE_A", compute_samples::to_string(t));
}

TEST(ZeImageFormatSwizzleToString, ZE_IMAGE_FORMAT_SWIZZLE_0) {
  const ze_image_format_swizzle_t t = ZE_IMAGE_FORMAT_SWIZZLE_0;
  EXPECT_EQ("ZE_IMAGE_FORMAT_SWIZZLE_0", compute_samples::to_string(t));
}

TEST(ZeImageFormatSwizzleToString, ZE_IMAGE_FORMAT_SWIZZLE_1) {
  const ze_image_format_swizzle_t t = ZE_IMAGE_FORMAT_SWIZZLE_1;
  EXPECT_EQ("ZE_IMAGE_FORMAT_SWIZZLE_1", compute_samples::to_string(t));
}

TEST(ZeImageFormatSwizzleToString, ZE_IMAGE_FORMAT_SWIZZLE_X) {
  const ze_image_format_swizzle_t t = ZE_IMAGE_FORMAT_SWIZZLE_X;
  EXPECT_EQ("ZE_IMAGE_FORMAT_SWIZZLE_X", compute_samples::to_string(t));
}

TEST(ZeImageFormatSwizzleToString, InvalidValue) {
  const int invalid_value = ZE_IMAGE_FORMAT_SWIZZLE_X + 1;
  const auto s = static_cast<ze_image_format_swizzle_t>(
      static_cast<uint32_t>(invalid_value));
  EXPECT_EQ("Unknown ze_image_format_swizzle_t value: " +
                std::to_string(invalid_value),
            compute_samples::to_string(s));
}

TEST(ZeImageFlagToString, ZE_IMAGE_FLAG_PROGRAM_READ) {
  const ze_image_flag_t f = ZE_IMAGE_FLAG_PROGRAM_READ;
  EXPECT_EQ("ZE_IMAGE_FLAG_PROGRAM_READ", compute_samples::to_string(f));
}

TEST(ZeImageFlagToString, ZE_IMAGE_FLAG_PROGRAM_WRITE) {
  const ze_image_flag_t f = ZE_IMAGE_FLAG_PROGRAM_WRITE;
  EXPECT_EQ("ZE_IMAGE_FLAG_PROGRAM_WRITE", compute_samples::to_string(f));
}

TEST(ZeImageFlagToString, ZE_IMAGE_FLAG_BIAS_CACHED) {
  const ze_image_flag_t f = ZE_IMAGE_FLAG_BIAS_CACHED;
  EXPECT_EQ("ZE_IMAGE_FLAG_BIAS_CACHED", compute_samples::to_string(f));
}

TEST(ZeImageFlagToString, ZE_IMAGE_FLAG_BIAS_UNCACHED) {
  const ze_image_flag_t f = ZE_IMAGE_FLAG_BIAS_UNCACHED;
  EXPECT_EQ("ZE_IMAGE_FLAG_BIAS_UNCACHED", compute_samples::to_string(f));
}

TEST(ZeImageFlagToString, ZE_IMAGE_FLAG_PROGRAM_READ_WRITE) {
  const auto f = static_cast<ze_image_flag_t>(static_cast<uint32_t>(
      ZE_IMAGE_FLAG_PROGRAM_READ | ZE_IMAGE_FLAG_PROGRAM_WRITE));
  EXPECT_EQ("ZE_IMAGE_FLAG_PROGRAM_READ | ZE_IMAGE_FLAG_PROGRAM_WRITE",
            compute_samples::to_string(f));
}

TEST(ZeImageFlagToString, ZE_IMAGE_FLAG_PROGRAM_READ_CACHED) {
  const auto f = static_cast<ze_image_flag_t>(static_cast<uint32_t>(
      ZE_IMAGE_FLAG_PROGRAM_READ | ZE_IMAGE_FLAG_BIAS_CACHED));
  EXPECT_EQ("ZE_IMAGE_FLAG_PROGRAM_READ | ZE_IMAGE_FLAG_BIAS_CACHED",
            compute_samples::to_string(f));
}

TEST(ZeImageFlagToString, ZE_IMAGE_FLAG_PROGRAM_READ_UNCACHED) {
  const auto f = static_cast<ze_image_flag_t>(static_cast<uint32_t>(
      ZE_IMAGE_FLAG_PROGRAM_READ | ZE_IMAGE_FLAG_BIAS_UNCACHED));
  EXPECT_EQ("ZE_IMAGE_FLAG_PROGRAM_READ | ZE_IMAGE_FLAG_BIAS_UNCACHED",
            compute_samples::to_string(f));
}

TEST(ZeImageFlagToString, ZE_IMAGE_FLAG_PROGRAM_WRITE_CACHED) {
  const auto f = static_cast<ze_image_flag_t>(static_cast<uint32_t>(
      ZE_IMAGE_FLAG_PROGRAM_WRITE | ZE_IMAGE_FLAG_BIAS_CACHED));
  EXPECT_EQ("ZE_IMAGE_FLAG_PROGRAM_WRITE | ZE_IMAGE_FLAG_BIAS_CACHED",
            compute_samples::to_string(f));
}

TEST(ZeImageFlagToString, ZE_IMAGE_FLAG_PROGRAM_WRITE_UNCACHED) {
  const auto f = static_cast<ze_image_flag_t>(static_cast<uint32_t>(
      ZE_IMAGE_FLAG_PROGRAM_WRITE | ZE_IMAGE_FLAG_BIAS_UNCACHED));
  EXPECT_EQ("ZE_IMAGE_FLAG_PROGRAM_WRITE | ZE_IMAGE_FLAG_BIAS_UNCACHED",
            compute_samples::to_string(f));
}

TEST(ZeImageFlagToString, ZE_IMAGE_FLAG_PROGRAM_READ_WRITE_CACHED) {
  const auto f = static_cast<ze_image_flag_t>(static_cast<uint32_t>(
      ZE_IMAGE_FLAG_PROGRAM_READ | ZE_IMAGE_FLAG_PROGRAM_WRITE |
      ZE_IMAGE_FLAG_BIAS_CACHED));
  EXPECT_EQ("ZE_IMAGE_FLAG_PROGRAM_READ | ZE_IMAGE_FLAG_PROGRAM_WRITE | "
            "ZE_IMAGE_FLAG_BIAS_CACHED",
            compute_samples::to_string(f));
}

TEST(ZeImageFlagToString, ZE_IMAGE_FLAG_PROGRAM_READ_WRITE_UNCACHED) {
  const auto f = static_cast<ze_image_flag_t>(static_cast<uint32_t>(
      ZE_IMAGE_FLAG_PROGRAM_READ | ZE_IMAGE_FLAG_PROGRAM_WRITE |
      ZE_IMAGE_FLAG_BIAS_UNCACHED));
  EXPECT_EQ("ZE_IMAGE_FLAG_PROGRAM_READ | ZE_IMAGE_FLAG_PROGRAM_WRITE | "
            "ZE_IMAGE_FLAG_BIAS_UNCACHED",
            compute_samples::to_string(f));
}

TEST(ZeImageTypeToString, ZE_IMAGE_TYPE_1D) {
  const ze_image_type_t t = ZE_IMAGE_TYPE_1D;
  EXPECT_EQ("ZE_IMAGE_TYPE_1D", compute_samples::to_string(t));
}

TEST(ZeImageTypeToString, ZE_IMAGE_TYPE_2D) {
  const ze_image_type_t t = ZE_IMAGE_TYPE_2D;
  EXPECT_EQ("ZE_IMAGE_TYPE_2D", compute_samples::to_string(t));
}

TEST(ZeImageTypeToString, ZE_IMAGE_TYPE_3D) {
  const ze_image_type_t t = ZE_IMAGE_TYPE_3D;
  EXPECT_EQ("ZE_IMAGE_TYPE_3D", compute_samples::to_string(t));
}

TEST(ZeImageTypeToString, ZE_IMAGE_TYPE_1DARRAY) {
  const ze_image_type_t t = ZE_IMAGE_TYPE_1DARRAY;
  EXPECT_EQ("ZE_IMAGE_TYPE_1DARRAY", compute_samples::to_string(t));
}
TEST(ZeImageTypeToString, ZE_IMAGE_TYPE_2DARRAY) {
  const ze_image_type_t t = ZE_IMAGE_TYPE_2DARRAY;
  EXPECT_EQ("ZE_IMAGE_TYPE_2DARRAY", compute_samples::to_string(t));
}

TEST(ZeImageTypeToString, InvalidValue) {
  const int invalid_value = ZE_IMAGE_TYPE_3D + 1;
  const auto t =
      static_cast<ze_image_type_t>(static_cast<uint32_t>(invalid_value));
  EXPECT_EQ("Unknown ze_image_type_t value: " + std::to_string(invalid_value),
            compute_samples::to_string(t));
}

TEST(ZeDeviceTypeToString, ZE_DEVICE_TYPE_GPU) {
  const ze_device_type_t t = ZE_DEVICE_TYPE_GPU;
  EXPECT_EQ("ZE_DEVICE_TYPE_GPU", compute_samples::to_string(t));
}

TEST(ZeDeviceTypeToString, ZE_DEVICE_TYPE_FPGA) {
  const ze_device_type_t t = ZE_DEVICE_TYPE_FPGA;
  EXPECT_EQ("ZE_DEVICE_TYPE_FPGA", compute_samples::to_string(t));
}

TEST(ZeDeviceTypeToString, InvalidValue) {
  const int invalid_value = ZE_DEVICE_TYPE_FPGA + 1;
  const auto t =
      static_cast<ze_device_type_t>(static_cast<uint32_t>(invalid_value));
  EXPECT_EQ("Unknown ze_device_type_t value: " + std::to_string(invalid_value),
            compute_samples::to_string(t));
}

TEST(ZeDeviceUUIDToString, ValidValue) {
  const ze_device_uuid_t uuid = {0x78, 0x56, 0x34, 0x12, 0x78, 0x56,
                                 0x34, 0x12, 0x78, 0x56, 0x34, 0x12,
                                 0x78, 0x56, 0x34, 0x12};
  EXPECT_EQ("12345678-1234-5678-1234-567812345678",
            compute_samples::to_string(uuid));
}

TEST(ZeMemoryAccessCapabilitiesToString, WithAccess) {
  const auto c = static_cast<ze_memory_access_capabilities_t>(
      ZE_MEMORY_ACCESS | ZE_MEMORY_ATOMIC_ACCESS);
  EXPECT_EQ("ZE_MEMORY_ACCESS | ZE_MEMORY_ATOMIC_ACCESS",
            compute_samples::to_string(c));
}

TEST(ZeMemoryAccessCapabilitiesToString, WithoutAccess) {
  const auto c =
      static_cast<ze_memory_access_capabilities_t>(ZE_MEMORY_ACCESS_NONE);
  EXPECT_EQ("ZE_MEMORY_ACCESS_NONE", compute_samples::to_string(c));
}

TEST(ZeDriverUUIDToString, ValidValue) {
  const ze_driver_uuid_t uuid = {0x78, 0x56, 0x34, 0x12, 0x78, 0x56,
                                 0x34, 0x12, 0x78, 0x56, 0x34, 0x12,
                                 0x78, 0x56, 0x34, 0x12};
  EXPECT_EQ("12345678-1234-5678-1234-567812345678",
            compute_samples::to_string(uuid));
}

TEST(ZeNativeKernelUUIDToString, ValidValue) {
  const ze_native_kernel_uuid_t uuid = {0x78, 0x56, 0x34, 0x12, 0x78, 0x56,
                                        0x34, 0x12, 0x78, 0x56, 0x34, 0x12,
                                        0x78, 0x56, 0x34, 0x12};
  EXPECT_EQ("12345678-1234-5678-1234-567812345678",
            compute_samples::to_string(uuid));
}

TEST(ZeFloatingPointCapabilitiesToString, All) {
  const auto c = static_cast<ze_fp_capabilities_t>(
      ZE_FP_CAPS_DENORM | ZE_FP_CAPS_INF_NAN | ZE_FP_CAPS_ROUND_TO_NEAREST |
      ZE_FP_CAPS_ROUND_TO_ZERO | ZE_FP_CAPS_ROUND_TO_INF | ZE_FP_CAPS_FMA |
      ZE_FP_CAPS_ROUNDED_DIVIDE_SQRT | ZE_FP_CAPS_SOFT_FLOAT);
  EXPECT_EQ("ZE_FP_CAPS_DENORM | ZE_FP_CAPS_INF_NAN | "
            "ZE_FP_CAPS_ROUND_TO_NEAREST | ZE_FP_CAPS_ROUND_TO_ZERO | "
            "ZE_FP_CAPS_ROUND_TO_INF | ZE_FP_CAPS_FMA | "
            "ZE_FP_CAPS_ROUNDED_DIVIDE_SQRT | ZE_FP_CAPS_SOFT_FLOAT",
            compute_samples::to_string(c));
}

TEST(ZeFloatingPointCapabilitiesToString, None) {
  const auto c = static_cast<ze_fp_capabilities_t>(ZE_FP_CAPS_NONE);
  EXPECT_EQ("ZE_FP_CAPS_NONE", compute_samples::to_string(c));
}
