/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_cl_unified_shared_memory/common.hpp"
#include "test_harness/test_harness.hpp"

namespace compute = boost::compute;

namespace {

const std::vector<std::vector<cl_mem_properties_intel>>
    host_combinations_cl_mem_properties_intel = {{0}};

class clHostMemAllocINTELWithMemoryProperties
    : public ::testing::TestWithParam<std::vector<cl_mem_properties_intel>> {};

HWTEST_P(clHostMemAllocINTELWithMemoryProperties,
         GivenMemoryPropertiesThenReturnsNotNullPointer) {
  const compute::usm_type memory_type = compute::usm_type::host;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  compute::context context(compute::system::default_context());
  const std::vector<cl_mem_properties_intel> properties = GetParam();
  cl_int *p = nullptr;
  p = compute::host_mem_alloc<cl_int>(context, properties.data(), 1, 0);
  EXPECT_NE(nullptr, p);
  compute::mem_free(context, p);
}

INSTANTIATE_TEST_SUITE_P(
    MemoryProperties, clHostMemAllocINTELWithMemoryProperties,
    ::testing::ValuesIn(host_combinations_cl_mem_properties_intel));

const std::vector<std::vector<cl_mem_properties_intel>>
    device_combinations_cl_mem_properties_intel = {
        {0}, {CL_MEM_ALLOC_FLAGS_INTEL, CL_MEM_ALLOC_WRITE_COMBINED_INTEL, 0}};

class clDeviceMemAllocINTELWithMemoryProperties
    : public ::testing::TestWithParam<std::vector<cl_mem_properties_intel>> {};

HWTEST_P(clDeviceMemAllocINTELWithMemoryProperties,
         GivenMemoryPropertiesThenReturnsNotNullPointer) {
  const compute::usm_type memory_type = compute::usm_type::device;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  compute::context context(compute::system::default_context());
  compute::device device = compute::system::default_device();
  const std::vector<cl_mem_properties_intel> properties = GetParam();
  cl_int *p = nullptr;
  p = compute::device_mem_alloc<cl_int>(context, device, properties.data(), 1,
                                        0);
  EXPECT_NE(nullptr, p);
  compute::mem_free(context, p);
}

INSTANTIATE_TEST_SUITE_P(
    MemoryProperties, clDeviceMemAllocINTELWithMemoryProperties,
    ::testing::ValuesIn(device_combinations_cl_mem_properties_intel));

const std::vector<std::vector<cl_mem_properties_intel>>
    shared_combinations_cl_mem_properties_intel = {{0}};

class clSharedMemAllocINTELWithMemoryProperties
    : public ::testing::TestWithParam<std::vector<cl_mem_properties_intel>> {};

HWTEST_P(clSharedMemAllocINTELWithMemoryProperties,
         GivenMemoryPropertiesThenReturnsNotNullPointer) {
  const compute::usm_type memory_type = compute::usm_type::shared;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  compute::context context(compute::system::default_context());
  compute::device device = compute::system::default_device();
  const std::vector<cl_mem_properties_intel> properties = GetParam();
  cl_int *p = nullptr;
  p = compute::shared_mem_alloc<cl_int>(context, device, properties.data(), 1,
                                        0);
  EXPECT_NE(nullptr, p);
  compute::mem_free(context, p);
}

INSTANTIATE_TEST_SUITE_P(
    MemoryProperties, clSharedMemAllocINTELWithMemoryProperties,
    ::testing::ValuesIn(shared_combinations_cl_mem_properties_intel));

const std::vector<cl_uint> valid_alignments = {
    1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};

class clHostMemAllocINTELWithAlignment
    : public ::testing::TestWithParam<cl_uint> {};

HWTEST_P(clHostMemAllocINTELWithAlignment, GivenAlignmentThenPointerIsAligned) {
  const compute::usm_type memory_type = compute::usm_type::host;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  compute::context context(compute::system::default_context());
  const cl_uint input_alignment = GetParam();
  auto *p =
      compute::host_mem_alloc<cl_int>(context, nullptr, 1, input_alignment);
  EXPECT_NE(nullptr, p);
  const auto address = reinterpret_cast<std::uintptr_t>(p);
  EXPECT_EQ(0, address % input_alignment);
  compute::mem_free(context, p);
}

INSTANTIATE_TEST_SUITE_P(Alignment, clHostMemAllocINTELWithAlignment,
                         ::testing::ValuesIn(valid_alignments));

class clDeviceMemAllocINTELWithAlignment
    : public ::testing::TestWithParam<cl_uint> {};

HWTEST_P(clDeviceMemAllocINTELWithAlignment,
         GivenAlignmentThenPointerIsAligned) {
  const compute::usm_type memory_type = compute::usm_type::device;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  compute::context context(compute::system::default_context());
  compute::device device = compute::system::default_device();
  const cl_uint input_alignment = GetParam();
  auto *p = compute::device_mem_alloc<cl_int>(context, device, nullptr, 1,
                                              input_alignment);
  EXPECT_NE(nullptr, p);
  const auto address = reinterpret_cast<std::uintptr_t>(p);
  EXPECT_EQ(0, address % input_alignment);
  compute::mem_free(context, p);
}

INSTANTIATE_TEST_SUITE_P(Alignment, clDeviceMemAllocINTELWithAlignment,
                         ::testing::ValuesIn(valid_alignments));

class clSharedMemAllocINTELWithAlignment
    : public ::testing::TestWithParam<cl_uint> {};

HWTEST_P(clSharedMemAllocINTELWithAlignment,
         GivenAlignmentThenPointerIsAligned) {
  const compute::usm_type memory_type = compute::usm_type::shared;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  compute::context context(compute::system::default_context());
  compute::device device = compute::system::default_device();
  const cl_uint input_alignment = GetParam();
  auto *p = compute::shared_mem_alloc<cl_int>(context, device, nullptr, 1,
                                              input_alignment);
  EXPECT_NE(nullptr, p);
  const auto address = reinterpret_cast<std::uintptr_t>(p);
  EXPECT_EQ(0, address % input_alignment);
  compute::mem_free(context, p);
}

INSTANTIATE_TEST_SUITE_P(Alignment, clSharedMemAllocINTELWithAlignment,
                         ::testing::ValuesIn(valid_alignments));

class clGetMemAllocInfoINTELWithAllocationFlags
    : public ::testing::TestWithParam<
          std::tuple<boost::compute::usm_type, cl_mem_alloc_flags_intel>> {
protected:
  void SetUp() override {
    const boost::compute::usm_type memory_type = std::get<0>(GetParam());
    memory_ = get_memory_wrapper<uint8_t>(memory_type);

    if (!is_memory_access_supported(memory_type)) {
      REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                  cs::to_string(memory_type));
    }

    flags_ = std::get<1>(GetParam());
    const std::vector<cl_mem_properties_intel> properties = {
        CL_MEM_ALLOC_FLAGS_INTEL, flags_, 0};
    memory_->allocate(1, properties);
  }

  void TearDown() override { memory_->free(); }

  std::unique_ptr<MemoryWrapper<uint8_t>> memory_;
  cl_mem_alloc_flags_intel flags_;
};

HWTEST_P(clGetMemAllocInfoINTELWithAllocationFlags,
         GivenAllocationFlagsParameterThenProperFlagsAreReturned) {
  compute::context context(compute::system::default_context());
  auto f = compute::get_mem_alloc_info<cl_mem_alloc_flags_intel>(
      context, memory_->get(), CL_MEM_ALLOC_FLAGS_INTEL);
  EXPECT_EQ(flags_, f);
}

const std::vector<cl_mem_alloc_flags_intel>
    host_memory_cl_mem_alloc_flags_intel = {};

INSTANTIATE_TEST_SUITE_P(
    HostMemory, clGetMemAllocInfoINTELWithAllocationFlags,
    ::testing::Combine(
        ::testing::Values(boost::compute::usm_type::host),
        ::testing::ValuesIn(host_memory_cl_mem_alloc_flags_intel)));

const std::vector<cl_mem_alloc_flags_intel>
    device_memory_cl_mem_alloc_flags_intel = {
        CL_MEM_ALLOC_WRITE_COMBINED_INTEL};

INSTANTIATE_TEST_SUITE_P(
    DeviceMemory, clGetMemAllocInfoINTELWithAllocationFlags,
    ::testing::Combine(
        ::testing::Values(boost::compute::usm_type::device),
        ::testing::ValuesIn(device_memory_cl_mem_alloc_flags_intel)));

const std::vector<cl_mem_alloc_flags_intel>
    shared_memory_cl_mem_alloc_flags_intel = {};

INSTANTIATE_TEST_SUITE_P(
    SharedMemory, clGetMemAllocInfoINTELWithAllocationFlags,
    ::testing::Combine(
        ::testing::Values(boost::compute::usm_type::shared),
        ::testing::ValuesIn(shared_memory_cl_mem_alloc_flags_intel)));

class clGetMemAllocInfoINTELWithDriverManagedMemory
    : public ::testing::TestWithParam<boost::compute::usm_type> {
protected:
  void SetUp() override {
    memory_type_ = GetParam();
    memory_ = get_memory_wrapper<uint8_t>(memory_type_);

    if (!is_memory_access_supported(memory_type_)) {
      REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                  cs::to_string(memory_type_));
    }

    memory_->allocate(size_);
  }

  void TearDown() override { memory_->free(); }

  std::unique_ptr<MemoryWrapper<uint8_t>> memory_;
  boost::compute::usm_type memory_type_;
  const size_t size_ = 8;
};

HWTEST_P(
    clGetMemAllocInfoINTELWithDriverManagedMemory,
    GivenBasePointerWithOffsetAndAllocationBasePointerParameterThenBasePointerIsReturned) {
  const uint8_t *base = memory_->get();
  const uint8_t *base_with_offset = base + 1;
  compute::context context(compute::system::default_context());
  auto *actual = compute::get_mem_alloc_info<uint8_t *>(
      context, base_with_offset, CL_MEM_ALLOC_BASE_PTR_INTEL);
  EXPECT_EQ(base, actual);
}

HWTEST_P(clGetMemAllocInfoINTELWithDriverManagedMemory,
         GivenAllocationSizeParameterThenProperSizeIsReturned) {
  compute::context context(compute::system::default_context());
  auto actual = compute::get_mem_alloc_info<size_t>(context, memory_->get(),
                                                    CL_MEM_ALLOC_SIZE_INTEL);
  EXPECT_EQ(size_ * sizeof(uint8_t), actual);
}

HWTEST_P(clGetMemAllocInfoINTELWithDriverManagedMemory,
         GivenAllocationTypeParameterThenProperTypeIsReturned) {
  compute::context context(compute::system::default_context());
  auto t = compute::get_mem_alloc_info<cl_unified_shared_memory_type_intel>(
      context, memory_->get(), CL_MEM_ALLOC_TYPE_INTEL);
  EXPECT_EQ(memory_type_, boost::compute::usm_type(t));
}

INSTANTIATE_TEST_SUITE_P(MemoryTypes,
                         clGetMemAllocInfoINTELWithDriverManagedMemory,
                         ::testing::Values(boost::compute::usm_type::host,
                                           boost::compute::usm_type::device,
                                           boost::compute::usm_type::shared));

class clGetMemAllocInfoINTELWithSystemManagedMemory : public ::testing::Test {
protected:
  void SetUp() override {
    memory_type_ = boost::compute::usm_type::unknown;
    memory_ = get_memory_wrapper<uint8_t>(memory_type_);
    memory_->allocate(8);
  }

  void TearDown() override { memory_->free(); }

  std::unique_ptr<MemoryWrapper<uint8_t>> memory_;
  boost::compute::usm_type memory_type_;
};

HWTEST_F(clGetMemAllocInfoINTELWithSystemManagedMemory,
         GivenAllocationTypeParameterThenProperTypeIsReturned) {
  compute::context context(compute::system::default_context());
  auto t = compute::get_mem_alloc_info<cl_unified_shared_memory_type_intel>(
      context, memory_->get(), CL_MEM_ALLOC_TYPE_INTEL);
  EXPECT_EQ(memory_type_, boost::compute::usm_type(t));
}

HWTEST(clGetMemAllocInfoINTELTests,
       GivenHostAllocationAndDeviceParameterThenNullIsReturned) {
  const compute::usm_type memory_type = compute::usm_type::host;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  compute::context context(compute::system::default_context());
  auto *p = compute::host_mem_alloc<cl_int>(context, nullptr, 1, 0);

  auto d = compute::get_mem_alloc_info<cl_device_id>(context, p,
                                                     CL_MEM_ALLOC_DEVICE_INTEL);
  EXPECT_EQ(nullptr, d);

  compute::mem_free(context, p);
}

HWTEST(clGetMemAllocInfoINTELTests,
       GivenDeviceAllocationAndDeviceParameterThenProperDeviceIsReturned) {
  const compute::usm_type memory_type = compute::usm_type::device;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  compute::context context(compute::system::default_context());
  compute::device device(compute::system::default_device());
  auto *p = compute::device_mem_alloc<cl_int>(context, device, nullptr, 1, 0);

  auto d = compute::get_mem_alloc_info<cl_device_id>(context, p,
                                                     CL_MEM_ALLOC_DEVICE_INTEL);
  EXPECT_EQ(device.id(), d);

  compute::mem_free(context, p);
}

HWTEST(
    clGetMemAllocInfoINTELTests,
    GivenSharedAllocationWithDeviceAndDeviceParameterThenProperDeviceIsReturned) {
  const compute::usm_type memory_type = compute::usm_type::shared;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  compute::context context(compute::system::default_context());
  compute::device device(compute::system::default_device());
  auto *p = compute::shared_mem_alloc<cl_int>(context, device, nullptr, 1, 0);

  auto d = compute::get_mem_alloc_info<cl_device_id>(context, p,
                                                     CL_MEM_ALLOC_DEVICE_INTEL);
  EXPECT_EQ(device.id(), d);

  compute::mem_free(context, p);
}

HWTEST(clGetMemAllocInfoINTELTests,
       GivenSharedAllocationWithoutDeviceAndDeviceParameterThenNullIsReturned) {
  const compute::usm_type memory_type = compute::usm_type::shared;
  if (!is_memory_access_supported(memory_type)) {
    REPORT_UNSUPPORTED_SCENARIO("Memory access is not supported for: " +
                                cs::to_string(memory_type));
  }

  compute::context context(compute::system::default_context());
  compute::device device;
  auto *p = compute::shared_mem_alloc<cl_int>(context, device, nullptr, 1, 0);

  auto d = compute::get_mem_alloc_info<cl_device_id>(context, p,
                                                     CL_MEM_ALLOC_DEVICE_INTEL);
  EXPECT_EQ(nullptr, d);

  compute::mem_free(context, p);
}

HWTEST(clGetMemAllocInfoINTELTests,
       GivenSharedSystemAllocationAndDeviceParameterThenNullIsReturned) {
  const compute::usm_type memory_type = compute::usm_type::unknown;

  std::unique_ptr<cl_int> p(new cl_int);

  compute::context context(compute::system::default_context());
  auto d = compute::get_mem_alloc_info<cl_device_id>(context, p.get(),
                                                     CL_MEM_ALLOC_DEVICE_INTEL);
  EXPECT_EQ(nullptr, d);
}

} // namespace
