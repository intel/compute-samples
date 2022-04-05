/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gmock/gmock.h"
#include "ocl_utils/ocl_utils.hpp"
#include "utils/utils.hpp"

#include <boost/compute/core.hpp>
#include <boost/compute/intel/device.hpp>
#include <boost/compute/intel/command_queue.hpp>
namespace compute = boost::compute;

#include "logging/logging.hpp"
#include "test_harness/test_harness.hpp"

namespace {

class DeviceFixture : public ::testing::Test {
protected:
  void SetUp() override {
    device_ = compute::device_intel(compute::system::default_device());
    if (!device_.supports_extension("cl_intel_command_queue_families")) {
      REPORT_UNSUPPORTED_SCENARIO(
          "cl_intel_command_queue_families extension not supported");
    }
    context_ = compute::system::default_context();
    family_properties_ = device_.queue_family_properties();
    LOG_DEBUG << "Command queue families: "
              << device_.queue_family_properties().size();
  }

  compute::device_intel device_;
  compute::context context_;
  std::vector<cl_queue_family_properties_intel> family_properties_;
};

class TestDeviceQueries : public DeviceFixture {};

HWTEST_F(TestDeviceQueries, ValidProperties) {
  const cl_command_queue_properties valid_properties =
      CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE;
  for (const auto &single_family_properties : family_properties_) {
    EXPECT_THAT(single_family_properties.properties & CL_QUEUE_PROFILING_ENABLE,
                ::testing::Eq(CL_QUEUE_PROFILING_ENABLE));
    EXPECT_THAT(single_family_properties.properties | valid_properties,
                ::testing::Eq(valid_properties));
  }
}

HWTEST_F(TestDeviceQueries, ValidCapabilities) {
  const cl_command_queue_capabilities_intel valid_capabilities =
      CL_QUEUE_CAPABILITY_CREATE_SINGLE_QUEUE_EVENTS_INTEL |
      CL_QUEUE_CAPABILITY_CREATE_CROSS_QUEUE_EVENTS_INTEL |
      CL_QUEUE_CAPABILITY_SINGLE_QUEUE_EVENT_WAIT_LIST_INTEL |
      CL_QUEUE_CAPABILITY_CROSS_QUEUE_EVENT_WAIT_LIST_INTEL |
      CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL |
      CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_RECT_INTEL |
      CL_QUEUE_CAPABILITY_MAP_BUFFER_INTEL |
      CL_QUEUE_CAPABILITY_FILL_BUFFER_INTEL |
      CL_QUEUE_CAPABILITY_TRANSFER_IMAGE_INTEL |
      CL_QUEUE_CAPABILITY_MAP_IMAGE_INTEL |
      CL_QUEUE_CAPABILITY_FILL_IMAGE_INTEL |
      CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_IMAGE_INTEL |
      CL_QUEUE_CAPABILITY_TRANSFER_IMAGE_BUFFER_INTEL |
      CL_QUEUE_CAPABILITY_MARKER_INTEL | CL_QUEUE_CAPABILITY_BARRIER_INTEL |
      CL_QUEUE_CAPABILITY_KERNEL_INTEL;
  for (const auto &single_family_properties : family_properties_) {
    EXPECT_THAT(single_family_properties.capabilities | valid_capabilities,
                valid_capabilities);
  }
}

HWTEST_F(TestDeviceQueries, ValidCount) {
  for (const auto &single_family_properties : family_properties_) {
    EXPECT_THAT(single_family_properties.count, ::testing::Ge(1));
  }
}

HWTEST_F(TestDeviceQueries, ValidName) {
  for (const auto &single_family_properties : family_properties_) {
    EXPECT_THAT(single_family_properties.name, ::testing::StrNe(""));
  }
}

HWTEST_F(TestDeviceQueries, QueueWithDefaultCapabilitiesIsReported) {
  bool found = false;
  for (const auto &single_family_properties : family_properties_) {
    if (single_family_properties.capabilities ==
        CL_QUEUE_DEFAULT_CAPABILITIES_INTEL) {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found);
}

class TestQueueQueries : public DeviceFixture {};

HWTEST_F(TestQueueQueries, ValidFamilyAndIndex) {
  for (cl_uint family = 0; family < family_properties_.size(); ++family) {
    for (cl_uint index = 0; index < family_properties_[family].count; ++index) {
      const std::vector<cl_queue_properties> properties = {
          CL_QUEUE_FAMILY_INTEL, family, CL_QUEUE_INDEX_INTEL, index, 0};
      const auto queue =
          compute::command_queue_intel(context_, device_, properties.data());
      EXPECT_THAT(queue.family(), ::testing::Eq(family));
      EXPECT_THAT(queue.index(), ::testing::Eq(index));
    }
  }
}

std::vector<cl_uint> default_queue_families(
    const std::vector<cl_queue_family_properties_intel> &families) {
  std::vector<cl_uint> output;
  for (cl_uint family = 0; family < families.size(); ++family) {
    if (families[family].capabilities == CL_QUEUE_DEFAULT_CAPABILITIES_INTEL) {
      output.push_back(family);
    }
  }
  return output;
}

HWTEST_F(TestQueueQueries, DefaultQueue) {
  const auto queue =
      compute::command_queue_intel(compute::system::default_queue());
  const auto family = queue.family();
  const auto default_families = default_queue_families(family_properties_);
  EXPECT_THAT(default_families, ::testing::Contains(family));
  const auto count = family_properties_[family].count;
  EXPECT_THAT(queue.index(),
              ::testing::AllOf(::testing::Ge(0), ::testing::Lt(count)));
}

class CommandQueueFixture : public DeviceFixture {
protected:
  void SetUp() override {
    DeviceFixture::SetUp();
    for (cl_uint family = 0; family < family_properties_.size(); ++family) {
      for (cl_uint index = 0; index < family_properties_[family].count;
           ++index) {
        std::vector<cl_queue_properties> queue_properties = {
            CL_QUEUE_FAMILY_INTEL, family, CL_QUEUE_INDEX_INTEL, index};
        if (properties_ != 0) {
          queue_properties.push_back(CL_QUEUE_PROPERTIES);
          queue_properties.push_back(cl_queue_properties(properties_));
        }
        queue_properties.push_back(0);
        command_queues_.emplace_back(context_, device_,
                                     queue_properties.data());
      }
    }
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO("No command queues found");
    }
  }

  std::vector<compute::command_queue_intel> command_queues_;
  cl_command_queue_properties properties_ = 0;
};

void filter_command_queues(
    std::vector<compute::command_queue_intel> &command_queues,
    const cl_command_queue_capabilities_intel capabilities) {
  command_queues.erase(
      std::remove_if(
          command_queues.begin(), command_queues.end(),
          [&capabilities](const compute::command_queue_intel &command_queue) {
            return !command_queue.are_capabilities_supported(capabilities);
          }),
      command_queues.end());
}

class TestTransferBufferCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    filter_command_queues(command_queues_,
                          CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO(
          "No command queues supporting "
          "CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL found");
    }
  }
};

HWTEST_F(TestTransferBufferCapability, clEnqueueReadWriteBuffer) {
  std::vector<cl_int> input(32, 123);
  std::vector<cl_int> output(input.size(), 0);
  compute::buffer buffer(context_, compute_samples::size_in_bytes(input));
  for (auto &command_queue : command_queues_) {
    command_queue.enqueue_write_buffer(
        buffer, 0, compute_samples::size_in_bytes(input), input.data());
    command_queue.enqueue_read_buffer(
        buffer, 0, compute_samples::size_in_bytes(output), output.data());
    EXPECT_THAT(output, ::testing::ElementsAreArray(input));
  }
}

HWTEST_F(TestTransferBufferCapability, clEnqueueCopyBuffer) {
  std::vector<cl_int> input(32, 123);
  std::vector<cl_int> output(input.size(), 0);
  compute::buffer src_buffer(context_, compute_samples::size_in_bytes(input));
  compute::buffer dst_buffer(context_, compute_samples::size_in_bytes(input));
  for (auto &command_queue : command_queues_) {
    command_queue.enqueue_write_buffer(
        src_buffer, 0, compute_samples::size_in_bytes(input), input.data());
    command_queue.enqueue_copy_buffer(src_buffer, dst_buffer, 0, 0,
                                      compute_samples::size_in_bytes(input));
    command_queue.enqueue_read_buffer(
        dst_buffer, 0, compute_samples::size_in_bytes(output), output.data());
    EXPECT_THAT(output, ::testing::ElementsAreArray(input));
  }
}

class TestTransferBufferRectCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    filter_command_queues(command_queues_,
                          CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_RECT_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO(
          "No command queues supporting "
          "CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_RECT_INTEL found");
    }
  }
};

HWTEST_F(TestTransferBufferRectCapability, clEnqueueReadWriteBufferRect) {
  std::vector<cl_int> input(32, 123);
  std::vector<cl_int> output(input.size(), 0);
  const size_t origin[3] = {0, 0, 0};
  const size_t region[3] = {
      static_cast<size_t>(compute_samples::size_in_bytes(input)), 1, 1};
  compute::buffer buffer(context_, compute_samples::size_in_bytes(input));
  for (auto &command_queue : command_queues_) {
    command_queue.enqueue_write_buffer_rect(buffer, origin, origin, region, 0,
                                            0, 0, 0, input.data());
    command_queue.enqueue_read_buffer_rect(buffer, origin, origin, region, 0, 0,
                                           0, 0, output.data());
    EXPECT_THAT(output, ::testing::ElementsAreArray(input));
  }
}

HWTEST_F(TestTransferBufferRectCapability, clEnqueueCopyBufferRect) {
  std::vector<cl_int> input(32, 123);
  std::vector<cl_int> output(input.size(), 0);
  const size_t origin[3] = {0, 0, 0};
  const size_t region[3] = {
      static_cast<size_t>(compute_samples::size_in_bytes(input)), 1, 1};
  compute::buffer src_buffer(context_, compute_samples::size_in_bytes(input));
  compute::buffer dst_buffer(context_, compute_samples::size_in_bytes(input));
  for (auto &command_queue : command_queues_) {
    command_queue.enqueue_write_buffer_rect(src_buffer, origin, origin, region,
                                            0, 0, 0, 0, input.data());
    command_queue.enqueue_copy_buffer_rect(src_buffer, dst_buffer, origin,
                                           origin, region, 0, 0, 0, 0);
    command_queue.enqueue_read_buffer_rect(dst_buffer, origin, origin, region,
                                           0, 0, 0, 0, output.data());
    EXPECT_THAT(output, ::testing::ElementsAreArray(input));
  }
}

class TestMapBufferCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    filter_command_queues(command_queues_,
                          CL_QUEUE_CAPABILITY_MAP_BUFFER_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO("No command queues supporting "
                                  "CL_QUEUE_CAPABILITY_MAP_BUFFER_INTEL found");
    }
  }
};

HWTEST_F(TestMapBufferCapability, clEnqueueMapUnmapBuffer) {
  std::vector<cl_int> input(32, 123);
  std::vector<cl_int> output(input.size(), 0);
  compute::buffer buffer(context_, compute_samples::size_in_bytes(input),
                         CL_MEM_COPY_HOST_PTR, input.data());
  for (auto &command_queue : command_queues_) {
    auto ptr = static_cast<cl_int *>(command_queue.enqueue_map_buffer(
        buffer, CL_MAP_READ, 0, compute_samples::size_in_bytes(input)));
    std::copy(ptr, ptr + input.size(), output.data());
    command_queue.enqueue_unmap_buffer(buffer, ptr);
    EXPECT_THAT(output, ::testing::ElementsAreArray(input));
  }
}

class TestFillBufferCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    filter_command_queues(command_queues_,
                          CL_QUEUE_CAPABILITY_FILL_BUFFER_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO(
          "No command queues supporting CL_QUEUE_CAPABILITY_FILL_BUFFER_INTEL "
          "found");
    }
  }
};

HWTEST_F(TestFillBufferCapability, clEnqueueFillBuffer) {
  const cl_int pattern = 123;
  std::vector<cl_int> input(32, pattern);
  compute::buffer buffer(context_, compute_samples::size_in_bytes(input));
  for (auto &command_queue : command_queues_) {
    command_queue.enqueue_fill_buffer(buffer, &pattern, sizeof(pattern), 0,
                                      compute_samples::size_in_bytes(input));
    if (command_queue.are_capabilities_supported(
            CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL)) {
      std::vector<cl_int> output(input.size(), 0);
      command_queue.enqueue_read_buffer(
          buffer, 0, compute_samples::size_in_bytes(output), output.data());
      EXPECT_THAT(output, ::testing::ElementsAreArray(input));
    }
  }
}

class TestTransferImageCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    const auto are_images_supported =
        device_.get_info<cl_bool>(CL_DEVICE_IMAGE_SUPPORT);
    if (are_images_supported == 0) {
      REPORT_UNSUPPORTED_SCENARIO("Images not supported");
    }
    filter_command_queues(command_queues_,
                          CL_QUEUE_CAPABILITY_TRANSFER_IMAGE_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO(
          "No command queues supporting "
          "CL_QUEUE_CAPABILITY_TRANSFER_IMAGE_INTEL found");
    }
  }
};

HWTEST_F(TestTransferImageCapability, clEnqueueReadWriteImage) {
  std::vector<cl_uint> input(32, 123);
  std::vector<cl_uint> output(input.size(), 0);
  compute::image_format image_format(
      compute::image_format::channel_order::r,
      compute::image_format::channel_data_type::unsigned_int32);
  compute::image1d image(context_, input.size(), image_format,
                         CL_MEM_COPY_HOST_PTR, input.data());
  const size_t origin[3] = {0, 0, 0};
  const size_t region[3] = {input.size(), 1, 1};
  for (auto &command_queue : command_queues_) {
    command_queue.enqueue_write_image(image, origin, region, input.data());
    command_queue.enqueue_read_image(image, origin, region, 0, 0,
                                     output.data());
    EXPECT_THAT(output, ::testing::ElementsAreArray(input));
  }
}

HWTEST_F(TestTransferImageCapability, clEnqueueCopyImage) {
  std::vector<cl_uint> input(32, 123);
  std::vector<cl_uint> output(input.size(), 0);
  compute::image_format image_format(
      compute::image_format::channel_order::r,
      compute::image_format::channel_data_type::unsigned_int32);
  compute::image1d src_image(context_, input.size(), image_format,
                             CL_MEM_COPY_HOST_PTR, input.data());
  compute::image1d dst_image(context_, input.size(), image_format, 0);
  const size_t origin[3] = {0, 0, 0};
  const size_t region[3] = {input.size(), 1, 1};
  for (auto &command_queue : command_queues_) {
    command_queue.enqueue_write_image(src_image, origin, region, input.data());
    command_queue.enqueue_copy_image(src_image, dst_image, origin, origin,
                                     region);
    command_queue.enqueue_read_image(dst_image, origin, region, 0, 0,
                                     output.data());
    EXPECT_THAT(output, ::testing::ElementsAreArray(input));
  }
}

class TestMapImageCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    const auto are_images_supported =
        device_.get_info<cl_bool>(CL_DEVICE_IMAGE_SUPPORT);
    if (are_images_supported == 0) {
      REPORT_UNSUPPORTED_SCENARIO("Images not supported");
    }
    filter_command_queues(command_queues_, CL_QUEUE_CAPABILITY_MAP_IMAGE_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO("No command queues supporting "
                                  "CL_QUEUE_CAPABILITY_MAP_IMAGE_INTEL found");
    }
  }
};

HWTEST_F(TestMapImageCapability, clEnqueueMapUnmapImage) {
  std::vector<cl_uint> input(32, 123);
  std::vector<cl_uint> output(input.size(), 0);
  compute::image_format image_format(
      compute::image_format::channel_order::r,
      compute::image_format::channel_data_type::unsigned_int32);
  compute::image1d image(context_, input.size(), image_format,
                         CL_MEM_COPY_HOST_PTR, input.data());
  const size_t origin[3] = {0, 0, 0};
  const size_t region[3] = {input.size(), 1, 1};
  size_t row_pitch = 0;
  size_t slice_pitch = 0;
  for (auto &command_queue : command_queues_) {
    auto ptr = static_cast<cl_uint *>(command_queue.enqueue_map_image(
        image, CL_MAP_READ, origin, region, row_pitch, slice_pitch));
    std::copy(ptr, ptr + input.size(), output.data());
    command_queue.enqueue_unmap_image(image, ptr);
    EXPECT_THAT(output, ::testing::ElementsAreArray(input));
  }
}

class TestFillImageCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    const auto are_images_supported =
        device_.get_info<cl_bool>(CL_DEVICE_IMAGE_SUPPORT);
    if (are_images_supported == 0) {
      REPORT_UNSUPPORTED_SCENARIO("Images not supported");
    }
    filter_command_queues(command_queues_,
                          CL_QUEUE_CAPABILITY_FILL_IMAGE_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO("No command queues supporting "
                                  "CL_QUEUE_CAPABILITY_FILL_IMAGE_INTEL found");
    }
  }
};

HWTEST_F(TestFillImageCapability, clEnqueueFillImage) {
  const cl_uint color = 123;
  std::vector<cl_uint> input(32, color);
  compute::image_format image_format(
      compute::image_format::channel_order::r,
      compute::image_format::channel_data_type::unsigned_int32);
  compute::image1d image(context_, input.size(), image_format, 0);
  const size_t origin[3] = {0, 0, 0};
  const size_t region[3] = {input.size(), 1, 1};
  for (auto &command_queue : command_queues_) {
    command_queue.enqueue_fill_image(image, &color, origin, region);
    command_queue.finish();
    if (command_queue.are_capabilities_supported(
            CL_QUEUE_CAPABILITY_TRANSFER_IMAGE_INTEL)) {
      std::vector<cl_uint> output(input.size(), 0);
      command_queue.enqueue_read_image(image, origin, region, 0, 0,
                                       output.data());
      EXPECT_THAT(output, ::testing::ElementsAreArray(input));
    }
  }
}

class TestTransferBufferImageCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    const auto are_images_supported =
        device_.get_info<cl_bool>(CL_DEVICE_IMAGE_SUPPORT);
    if (are_images_supported == 0) {
      REPORT_UNSUPPORTED_SCENARIO("Images not supported");
    }
    filter_command_queues(command_queues_,
                          CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_IMAGE_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO(
          "No command queues supporting "
          "CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_IMAGE_INTEL found");
    }
  }
};

HWTEST_F(TestTransferBufferImageCapability, clEnqueueCopyBufferToImage) {
  std::vector<cl_uint> input(32, 123);
  compute::buffer buffer(context_, compute_samples::size_in_bytes(input),
                         CL_MEM_COPY_HOST_PTR, input.data());
  compute::image_format image_format(
      compute::image_format::channel_order::r,
      compute::image_format::channel_data_type::unsigned_int32);
  compute::image1d image(context_, input.size(), image_format, 0);
  const size_t origin[3] = {0, 0, 0};
  const size_t region[3] = {input.size(), 1, 1};
  for (auto &command_queue : command_queues_) {
    command_queue.enqueue_copy_buffer_to_image(buffer, image, 0, origin,
                                               region);
    if (command_queue.are_capabilities_supported(
            CL_QUEUE_CAPABILITY_TRANSFER_IMAGE_INTEL)) {
      std::vector<cl_uint> output(input.size(), 0);
      command_queue.enqueue_read_image(image, origin, region, 0, 0,
                                       output.data());
      EXPECT_THAT(output, ::testing::ElementsAreArray(input));
    }
  }
}

class TestTransferImageBufferCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    const auto are_images_supported =
        device_.get_info<cl_bool>(CL_DEVICE_IMAGE_SUPPORT);
    if (are_images_supported == 0) {
      REPORT_UNSUPPORTED_SCENARIO("Images not supported");
    }
    filter_command_queues(command_queues_,
                          CL_QUEUE_CAPABILITY_TRANSFER_IMAGE_BUFFER_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO(
          "No command queues supporting "
          "CL_QUEUE_CAPABILITY_TRANSFER_IMAGE_BUFFER_INTEL found");
    }
  }
};

HWTEST_F(TestTransferImageBufferCapability, clEnqueueCopyImageToBuffer) {
  std::vector<cl_uint> input(32, 123);
  compute::image_format image_format(
      compute::image_format::channel_order::r,
      compute::image_format::channel_data_type::unsigned_int32);
  compute::image1d image(context_, input.size(), image_format,
                         CL_MEM_COPY_HOST_PTR, input.data());
  compute::buffer buffer(context_, compute_samples::size_in_bytes(input));
  const size_t origin[3] = {0, 0, 0};
  const size_t region[3] = {input.size(), 1, 1};
  for (auto &command_queue : command_queues_) {
    command_queue.enqueue_copy_image_to_buffer(image, buffer, origin, region,
                                               0);
    if (command_queue.are_capabilities_supported(
            CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL)) {
      std::vector<cl_uint> output(input.size(), 0);
      command_queue.enqueue_read_buffer(
          buffer, 0, compute_samples::size_in_bytes(output), output.data());
      EXPECT_THAT(output, ::testing::ElementsAreArray(input));
    }
  }
}

class TestMarkerCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    filter_command_queues(command_queues_, CL_QUEUE_CAPABILITY_MARKER_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO("No command queues supporting "
                                  "CL_QUEUE_CAPABILITY_MARKER_INTEL found");
    }
  }
};

HWTEST_F(TestMarkerCapability, clEnqueueMarkerWithWaitList) {
  std::vector<cl_uint> input(32, 123);
  std::vector<cl_uint> output(input.size(), 0);
  compute::buffer input_buffer(context_, compute_samples::size_in_bytes(input),
                               CL_MEM_COPY_HOST_PTR, input.data());
  compute::buffer output_buffer(context_,
                                compute_samples::size_in_bytes(output));
  for (auto &command_queue : command_queues_) {
    if (command_queue.are_capabilities_supported(
            CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL)) {
      command_queue.enqueue_copy_buffer(input_buffer, output_buffer, 0, 0,
                                        compute_samples::size_in_bytes(input));
    }

    command_queue.enqueue_marker();

    if (command_queue.are_capabilities_supported(
            CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL)) {
      std::vector<cl_uint> output(input.size(), 0);
      command_queue.enqueue_read_buffer(output_buffer, 0,
                                        compute_samples::size_in_bytes(output),
                                        output.data());
      EXPECT_THAT(output, ::testing::ElementsAreArray(input));
    }
  }
}

class TestBarrierCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    filter_command_queues(command_queues_, CL_QUEUE_CAPABILITY_BARRIER_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO("No command queues supporting "
                                  "CL_QUEUE_CAPABILITY_BARRIER_INTEL found");
    }
  }
};

HWTEST_F(TestBarrierCapability, clEnqueueBarrierWithWaitList) {
  std::vector<cl_uint> input(32, 123);
  std::vector<cl_uint> output(input.size(), 0);
  compute::buffer input_buffer(context_, compute_samples::size_in_bytes(input),
                               CL_MEM_COPY_HOST_PTR, input.data());
  compute::buffer output_buffer(context_,
                                compute_samples::size_in_bytes(output));
  for (auto &command_queue : command_queues_) {
    if (command_queue.are_capabilities_supported(
            CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL)) {
      command_queue.enqueue_copy_buffer(input_buffer, output_buffer, 0, 0,
                                        compute_samples::size_in_bytes(input));
    }

    command_queue.enqueue_barrier();

    if (command_queue.are_capabilities_supported(
            CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL)) {
      std::vector<cl_uint> output(input.size(), 0);
      command_queue.enqueue_read_buffer(output_buffer, 0,
                                        compute_samples::size_in_bytes(output),
                                        output.data());
      EXPECT_THAT(output, ::testing::ElementsAreArray(input));
    }
  }
}

class TestKernelCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    filter_command_queues(command_queues_, CL_QUEUE_CAPABILITY_KERNEL_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO("No command queues supporting "
                                  "CL_QUEUE_CAPABILITY_KERNEL_INTEL found");
    }
    input_ = std::vector<cl_uint>(32, 123);
    output_ = std::vector<cl_uint>(input_.size(), 0);
    input_buffer_ =
        compute::buffer(context_, compute_samples::size_in_bytes(input_),
                        CL_MEM_COPY_HOST_PTR, input_.data());
    output_buffer_ =
        compute::buffer(context_, compute_samples::size_in_bytes(output_));

    const compute::program program = compute_samples::build_program(
        context_, "test_cl_command_queue_families.cl");
    kernel_ = program.create_kernel("copy_kernel");
    kernel_.set_args(input_buffer_, output_buffer_);
  }

  std::vector<cl_uint> input_;
  std::vector<cl_uint> output_;
  compute::buffer input_buffer_;
  compute::buffer output_buffer_;
  compute::kernel kernel_;
};

HWTEST_F(TestKernelCapability, clEnqueueNDRangeKernel) {
  for (auto &command_queue : command_queues_) {
    command_queue.enqueue_1d_range_kernel(kernel_, 0, input_.size(), 0);
    command_queue.finish();
    if (command_queue.are_capabilities_supported(
            CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL)) {
      command_queue.enqueue_read_buffer(output_buffer_, 0,
                                        compute_samples::size_in_bytes(output_),
                                        output_.data());
      EXPECT_THAT(output_, ::testing::ElementsAreArray(input_));
    }
  }
}

HWTEST_F(TestKernelCapability, clEnqueueTask) {
  for (auto &command_queue : command_queues_) {
    command_queue.enqueue_task(kernel_);
    command_queue.finish();
    if (command_queue.are_capabilities_supported(
            CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL)) {
      command_queue.enqueue_read_buffer(output_buffer_, 0, sizeof(cl_uint) * 1,
                                        output_.data());
      std::vector<cl_uint> reference(input_.size(), 0);
      reference[0] = input_[0];
      EXPECT_THAT(output_, ::testing::ElementsAreArray(reference));
    }
  }
}

HWTEST_F(TestKernelCapability, clEnqueueNativeKernel) {
  const auto exec_capabilities = device_.get_info<cl_device_exec_capabilities>(
      CL_DEVICE_EXECUTION_CAPABILITIES);
  if ((exec_capabilities & CL_EXEC_NATIVE_KERNEL) == 0) {
    REPORT_UNSUPPORTED_SCENARIO("CL_EXEC_NATIVE_KERNEL not supported");
  }
  // TODO
}

void test_profiling(const compute::event &event) {
  const auto queued = event.get_profiling_info<cl_ulong>(
      compute::event::profiling_info::profiling_command_queued);
  const auto submit = event.get_profiling_info<cl_ulong>(
      compute::event::profiling_info::profiling_command_submit);
  const auto start = event.get_profiling_info<cl_ulong>(
      compute::event::profiling_info::profiling_command_start);
  const auto end = event.get_profiling_info<cl_ulong>(
      compute::event::profiling_info::profiling_command_end);
  EXPECT_THAT(queued, ::testing::Lt(submit));
  EXPECT_THAT(submit, ::testing::Lt(start));
  EXPECT_THAT(start, ::testing::Lt(end));
}

class TestCreateSingleQueueEventsCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    properties_ = CL_QUEUE_PROFILING_ENABLE;
    CommandQueueFixture::SetUp();
    filter_command_queues(command_queues_,
                          CL_QUEUE_CAPABILITY_CREATE_SINGLE_QUEUE_EVENTS_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO(
          "No command queues supporting "
          "CL_QUEUE_CAPABILITY_CREATE_SINGLE_QUEUE_EVENTS_INTEL found");
    }
  }
};

HWTEST_F(TestCreateSingleQueueEventsCapability, clEnqueueReadBuffer) {
  filter_command_queues(command_queues_,
                        CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL);
  if (command_queues_.empty()) {
    REPORT_UNSUPPORTED_SCENARIO(
        "No command queues supporting "
        "CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL found");
  }
  std::vector<cl_uint> input(32, 123);
  std::vector<cl_uint> output(input.size(), 0);
  compute::buffer buffer(context_, compute_samples::size_in_bytes(input),
                         CL_MEM_COPY_HOST_PTR, input.data());
  for (auto &command_queue : command_queues_) {
    const compute::event event = command_queue.enqueue_read_buffer_async(
        buffer, 0, compute_samples::size_in_bytes(output), output.data());
    compute::wait_list(event).wait();
    EXPECT_THAT(output, ::testing::ElementsAreArray(input));
    test_profiling(event);
  }
}

class TestCreateCrossQueueEventsCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    properties_ = CL_QUEUE_PROFILING_ENABLE;
    CommandQueueFixture::SetUp();
    filter_command_queues(command_queues_,
                          CL_QUEUE_CAPABILITY_CREATE_CROSS_QUEUE_EVENTS_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO(
          "No command queues supporting "
          "CL_QUEUE_CAPABILITY_CREATE_CROSS_QUEUE_EVENTS_INTEL found");
    }
  }
};

HWTEST_F(TestCreateCrossQueueEventsCapability,
         CreateSingleQueueEventsCapabilityIsReported) {
  for (auto &command_queue : command_queues_) {
    EXPECT_THAT(command_queue.are_capabilities_supported(
                    CL_QUEUE_CAPABILITY_CREATE_CROSS_QUEUE_EVENTS_INTEL),
                ::testing::IsTrue());
  }
}

HWTEST_F(TestCreateCrossQueueEventsCapability, clEnqueueReadBuffer) {
  filter_command_queues(command_queues_,
                        CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL);
  if (command_queues_.empty()) {
    REPORT_UNSUPPORTED_SCENARIO(
        "No command queues supporting "
        "CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL found");
  }
  std::vector<cl_uint> input(32, 123);
  std::vector<cl_uint> output(input.size(), 0);
  compute::buffer buffer(context_, compute_samples::size_in_bytes(input),
                         CL_MEM_COPY_HOST_PTR, input.data());
  for (auto &command_queue : command_queues_) {
    const compute::event event = command_queue.enqueue_read_buffer_async(
        buffer, 0, compute_samples::size_in_bytes(output), output.data());
    compute::wait_list(event).wait();
    EXPECT_THAT(output, ::testing::ElementsAreArray(input));
    test_profiling(event);
  }
}

class TestSingleQueueEventWaitListCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    filter_command_queues(
        command_queues_,
        CL_QUEUE_CAPABILITY_SINGLE_QUEUE_EVENT_WAIT_LIST_INTEL |
            CL_QUEUE_CAPABILITY_CREATE_SINGLE_QUEUE_EVENTS_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO(
          "No command queues supporting "
          "CL_QUEUE_CAPABILITY_SINGLE_QUEUE_EVENT_WAIT_LIST_INTEL found");
    }
  }
};

HWTEST_F(TestSingleQueueEventWaitListCapability, clEnqueueReadWriteBuffer) {
  filter_command_queues(command_queues_,
                        CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL);
  if (command_queues_.empty()) {
    REPORT_UNSUPPORTED_SCENARIO(
        "No command queues supporting "
        "CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL found");
  }
  std::vector<cl_uint> input(32, 123);
  std::vector<cl_uint> output(input.size(), 0);
  compute::buffer buffer(context_, compute_samples::size_in_bytes(input));
  for (auto &command_queue : command_queues_) {
    const compute::event event_write = command_queue.enqueue_write_buffer_async(
        buffer, 0, compute_samples::size_in_bytes(input), input.data());
    const compute::event event_read = command_queue.enqueue_read_buffer_async(
        buffer, 0, compute_samples::size_in_bytes(output), output.data(),
        compute::wait_list(event_write));
    compute::wait_list(event_read).wait();
    EXPECT_THAT(output, ::testing::ElementsAreArray(input));
  }
}

class TestCrossQueueEventWaitListCapability : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    filter_command_queues(
        command_queues_,
        CL_QUEUE_CAPABILITY_CROSS_QUEUE_EVENT_WAIT_LIST_INTEL |
            CL_QUEUE_CAPABILITY_CREATE_CROSS_QUEUE_EVENTS_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO(
          "No command queues supporting "
          "CL_QUEUE_CAPABILITY_CROSS_QUEUE_EVENT_WAIT_LIST_INTEL|CL_QUEUE_"
          "CAPABILITY_CREATE_CROSS_QUEUE_EVENTS_INTEL found");
    }
  }
};

HWTEST_F(TestCrossQueueEventWaitListCapability, clEnqueueReadWriteBuffer) {
  filter_command_queues(command_queues_,
                        CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL);
  if (command_queues_.empty()) {
    REPORT_UNSUPPORTED_SCENARIO(
        "No command queues supporting "
        "CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL found");
  }
  std::vector<cl_uint> input(32, 123);
  std::vector<cl_uint> output(input.size(), 0);
  compute::buffer buffer(context_, compute_samples::size_in_bytes(input));
  for (auto &command_queue_write : command_queues_) {
    for (auto &command_queue_read : command_queues_) {
      const compute::event event_write =
          command_queue_write.enqueue_write_buffer_async(
              buffer, 0, compute_samples::size_in_bytes(input), input.data());
      const compute::event event_read =
          command_queue_read.enqueue_read_buffer_async(
              buffer, 0, compute_samples::size_in_bytes(output), output.data(),
              compute::wait_list(event_write));
      compute::wait_list(event_read).wait();
      EXPECT_THAT(output, ::testing::ElementsAreArray(input));
    }
  }
}

HWTEST_F(TestCrossQueueEventWaitListCapability,
         CrossQueueEventWaitListCapabilityIsReported) {
  for (auto &command_queue : command_queues_) {
    EXPECT_THAT(command_queue.are_capabilities_supported(
                    CL_QUEUE_CAPABILITY_SINGLE_QUEUE_EVENT_WAIT_LIST_INTEL),
                ::testing::IsTrue());
  }
}

class TestConcurrentExecution : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    filter_command_queues(command_queues_,
                          CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO(
          "No command queues supporting "
          "CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL found");
    }
    output_ = std::vector<cl_int>(command_queues_.size() * iterations_, 0);
    input_ = std::vector<cl_int>(command_queues_.size() * iterations_, 0);
    std::iota(input_.begin(), input_.end(), 0);
    for (size_t i = 0; i < output_.size(); ++i) {
      buffers_.emplace_back(context_, sizeof(cl_int));
    }
  }

  const int iterations_ = 10;
  std::vector<cl_int> input_;
  std::vector<cl_int> output_;
  std::vector<compute::buffer> buffers_;
};

HWTEST_F(TestConcurrentExecution, clEnqueueReadWriteBuffer) {
  for (int iteration = 0; iteration < iterations_; ++iteration) {
    for (size_t id = 0; id < command_queues_.size(); ++id) {
      const int offset = id * iterations_ + iteration;
      command_queues_[id].enqueue_write_buffer_async(
          buffers_[offset], 0, sizeof(cl_int), &input_[offset]);
      command_queues_[id].enqueue_read_buffer_async(
          buffers_[offset], 0, sizeof(cl_int), &output_[offset]);
    }
  }
  for (auto &command_queue : command_queues_) {
    command_queue.flush();
  }
  for (auto &command_queue : command_queues_) {
    command_queue.finish();
  }
  EXPECT_THAT(output_, ::testing::ElementsAreArray(input_));
}

class TestProducerConsumer : public CommandQueueFixture {
protected:
  void SetUp() override {
    CommandQueueFixture::SetUp();
    filter_command_queues(
        command_queues_,
        CL_QUEUE_CAPABILITY_KERNEL_INTEL |
            CL_QUEUE_CAPABILITY_TRANSFER_BUFFER_INTEL |
            CL_QUEUE_CAPABILITY_CREATE_CROSS_QUEUE_EVENTS_INTEL |
            CL_QUEUE_CAPABILITY_CROSS_QUEUE_EVENT_WAIT_LIST_INTEL);
    if (command_queues_.empty()) {
      REPORT_UNSUPPORTED_SCENARIO(
          "No command queues supporting "
          "CL_QUEUE_CAPABILITY_KERNEL_INTEL|CL_QUEUE_CAPABILITY_TRANSFER_"
          "BUFFER_INTEL|CL_QUEUE_CAPABILITY_CREATE_CROSS_QUEUE_EVENTS_INTEL|CL_"
          "QUEUE_CAPABILITY_CROSS_QUEUE_EVENT_WAIT_LIST_INTEL found");
    }
    input_ = std::vector<cl_int>(256);
    std::iota(input_.begin(), input_.end(), 0);
    for (size_t i = 0; i < command_queues_.size(); ++i) {
      buffers_.emplace_back(context_, compute_samples::size_in_bytes(input_));
    }
  }

  std::vector<cl_int> input_;
  std::vector<compute::buffer> buffers_;
};

HWTEST_F(TestProducerConsumer, clEnqueueCopyBuffer) {
  compute::event event;
  compute::wait_list wait_list;
  for (size_t i = 0; i < command_queues_.size(); ++i) {
    if (i == 0) {
      event = command_queues_[i].enqueue_write_buffer_async(
          buffers_[i], 0, compute_samples::size_in_bytes(input_),
          input_.data());
    } else {
      event = command_queues_[i].enqueue_copy_buffer(
          buffers_[i - 1], buffers_[i], 0, 0,
          compute_samples::size_in_bytes(input_), wait_list);
    }
    wait_list = {event};
  }
  for (auto &command_queue : command_queues_) {
    command_queue.finish();
  }
  for (const auto &buffer : buffers_) {
    std::vector<cl_int> output(input_.size(), 0);
    command_queues_[0].enqueue_read_buffer(
        buffer, 0, compute_samples::size_in_bytes(output), output.data());
    EXPECT_THAT(output, ::testing::ElementsAreArray(input_));
  }
}

} // namespace
