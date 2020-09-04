/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ocl_utils/ocl_utils.hpp"

#include <boost/compute/core.hpp>
#include <boost/compute/utility.hpp>
namespace compute = boost::compute;

#include "logging/logging.hpp"
#include "utils/utils.hpp"
#include "test_harness/test_harness.hpp"
#include "random/random.hpp"
namespace cs = compute_samples;

namespace {

#define CL_DEVICE_SUB_GROUP_SIZES_INTEL 0x4108

bool check_supported_subgroup_size(size_t sz) {
  compute::device device = compute::system::default_device();
  std::vector<size_t> intel_sg_sizes =
      device.get_info<std::vector<size_t>>(CL_DEVICE_SUB_GROUP_SIZES_INTEL);

  for (const auto s : intel_sg_sizes) {
    if (sz == s) {
      return true;
    }
  }

  return false;
}

HWTEST(TestCLVisaInjectionRtBasic, TemplateEmpty) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  std::vector<int> data(size);

  for (size_t i = 0; i < size; ++i) {
    data[i] = static_cast<int>(size - i);
  }

  const compute::context context = compute::system::default_context();

  compute::buffer in_buffer(context, cs::size_in_bytes(data),
                            compute::memory_object::read_only |
                                compute::memory_object::use_host_ptr,
                            data.data());
  compute::buffer out_buffer(context, cs::size_in_bytes(data),
                             compute::memory_object::write_only);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_template_empty.cl");

  compute::kernel kernel0 = program.create_kernel("test_template_empty0");
  kernel0.set_args(in_buffer, out_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_1d_range_kernel(kernel0, 0, data.size(), 0);

  {
    std::vector<int> result(size, 0);
    std::vector<int> expected(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }

  compute::kernel kernel1 = program.create_kernel("test_template_empty1");
  kernel1.set_args(in_buffer, out_buffer);

  queue.enqueue_1d_range_kernel(kernel1, 0, data.size(), 0);

  {
    std::vector<int> result(size, 0);

    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = (data[i] << 2) + 1;
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, TemplateNoOperands) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  std::vector<int> data(size);

  for (size_t i = 0; i < size; ++i) {
    data[i] = static_cast<int>(size - i);
  }

  const compute::context context = compute::system::default_context();

  compute::buffer in_buffer(context, cs::size_in_bytes(data),
                            compute::memory_object::read_only |
                                compute::memory_object::use_host_ptr,
                            data.data());
  compute::buffer out_buffer(context, cs::size_in_bytes(data),
                             compute::memory_object::write_only);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_template_no_operands.cl");

  compute::kernel kernel0 = program.create_kernel("test_template_no_operands0");
  kernel0.set_args(in_buffer, out_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_1d_range_kernel(kernel0, 0, data.size(), 0);

  {
    std::vector<int> result(size, 0);
    std::vector<int> expected(size, 0);

    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }

  compute::kernel kernel1 = program.create_kernel("test_template_no_operands1");
  kernel1.set_args(in_buffer, out_buffer);

  queue.enqueue_1d_range_kernel(kernel1, 0, data.size(), 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = (data[i] << 1) + 0xa5;
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, TemplateOperandsWithArbitraryOrder) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  std::vector<int> data(size);

  for (size_t i = 0; i < size; ++i) {
    data[i] = static_cast<int>(size - i);
  }

  const compute::context context = compute::system::default_context();

  compute::buffer in_buffer(context, cs::size_in_bytes(data),
                            compute::memory_object::read_only |
                                compute::memory_object::use_host_ptr,
                            data.data());
  compute::buffer out_buffer(context, cs::size_in_bytes(data),
                             compute::memory_object::write_only);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_template_operands_ao.cl");

  compute::kernel kernel =
      program.create_kernel("test_template_operands_arbitrary_order");
  kernel.set_args(out_buffer, in_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_1d_range_kernel(kernel, 0, data.size(), 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = data[i] - 0x42;
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, TemplateNoInputs) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const compute::context context = compute::system::default_context();

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::write_only);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_template_no_inputs.cl");

  compute::kernel kernel = program.create_kernel("test_template_no_inputs");
  kernel.set_args(out_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = (64 - i) * (64 - i);
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, TemplateNoOutputs) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const compute::context context = compute::system::default_context();

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::write_only);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_template_no_outputs.cl");

  compute::kernel kernel = program.create_kernel("test_template_no_outputs");
  kernel.set_args(out_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = (i << 4) + 0x42;
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, TemplateMultipleInstructions) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const int seed = 0;

  auto s1 = cs::generate_vector<int>(size, seed);
  auto s2 = cs::generate_vector<int>(size, seed);

  const compute::context context = compute::system::default_context();

  compute::buffer in1_buffer(context, cs::size_in_bytes(s1),
                             compute::memory_object::read_only |
                                 compute::memory_object::use_host_ptr,
                             s1.data());
  compute::buffer in2_buffer(context, cs::size_in_bytes(s2),
                             compute::memory_object::read_only |
                                 compute::memory_object::use_host_ptr,
                             s2.data());
  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::write_only);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_template_mltpl_insts.cl");

  compute::kernel kernel = program.create_kernel("test_template_mltpl_insts");
  kernel.set_args(out_buffer, in1_buffer, in2_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = (s1[0] - s1[i] * s2[i]) * (s2[i] - s1[i]);
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, TemplateVariableDeclaration) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const int seed = 0;

  auto s1 = cs::generate_vector<int>(size, seed);
  auto s2 = cs::generate_vector<int>(size, seed);

  const compute::context context = compute::system::default_context();

  compute::buffer in1_buffer(context, cs::size_in_bytes(s1),
                             compute::memory_object::read_only |
                                 compute::memory_object::use_host_ptr,
                             s1.data());
  compute::buffer in2_buffer(context, cs::size_in_bytes(s2),
                             compute::memory_object::read_only |
                                 compute::memory_object::use_host_ptr,
                             s2.data());
  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::write_only);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_template_var_decl.cl");

  compute::kernel kernel = program.create_kernel("test_template_var_decl");
  kernel.set_args(out_buffer, in1_buffer, in2_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = (s2[i >> 1] - s2[i] * s1[i]) * (s2[i] - s1[i]);
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, TemplateVariableScope) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const int seed = 0;

  auto s1 = cs::generate_vector<int>(size, seed);
  auto s2 = cs::generate_vector<int>(size, seed);

  const compute::context context = compute::system::default_context();

  compute::buffer in1_buffer(context, cs::size_in_bytes(s1),
                             compute::memory_object::read_only |
                                 compute::memory_object::use_host_ptr,
                             s1.data());
  compute::buffer in2_buffer(context, cs::size_in_bytes(s2),
                             compute::memory_object::read_only |
                                 compute::memory_object::use_host_ptr,
                             s2.data());
  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::write_only);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_template_var_scope.cl");

  compute::kernel kernel = program.create_kernel("test_template_var_scope");
  kernel.set_args(out_buffer, in1_buffer, in2_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = (s2[i] * s2[1] - s1[i]) + s1[i] * s2[i];
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, ConstraintsSameInputOutputWithPlus) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const int seed = 0;

  auto dst = cs::generate_vector<int>(size, seed);

  const compute::context context = compute::system::default_context();

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_constraints_same_io.cl");

  compute::kernel kernel =
      program.create_kernel("test_constraints_same_inout_with_plus");
  kernel.set_args(out_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst), dst.data());
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = dst[i] * dst[i];
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, ConstraintsSameInputOutputWithEqual) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const int seed = 0;

  auto dst = cs::generate_vector<int>(size, seed);

  const compute::context context = compute::system::default_context();

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_constraints_same_io.cl");

  compute::kernel kernel =
      program.create_kernel("test_constraints_same_inout_with_equal");
  kernel.set_args(out_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst), dst.data());
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = dst[i] + dst[i];
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, ConstraintsImmediateOperand) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const int seed = 0;

  const auto const_argument = cs::generate_value<uint32_t>(seed);
  auto dst = cs::generate_vector<int>(size, seed);

  const compute::context context = compute::system::default_context();

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  const std::string build_options =
      "-DCONST_ARGUMENT=" + std::to_string(const_argument);
  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_constraints_immediate.cl",
      build_options);

  compute::kernel kernel = program.create_kernel("test_constraints_immediate");
  kernel.set_args(out_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst), dst.data());
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = dst[i] | 1 << (sizeof(int) * 8 - 1) | 0x42;
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }

  if (check_supported_subgroup_size(8)) {
    kernel = program.create_kernel("test_constraints_immediate_simd8");

    auto src = cs::generate_vector<int>(size, seed);
    compute::buffer in_buffer(context, cs::size_in_bytes(src),
                              compute::memory_object::read_only |
                                  compute::memory_object::use_host_ptr,
                              src.data());
    kernel.set_args(in_buffer, out_buffer);

    queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst),
                               dst.data());
    queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

    {
      std::vector<int> result(size, 0);
      queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                                result.data());
      std::vector<int> expected(size);
      for (size_t i = 0; i < size; ++i) {
        expected[i] = src[i] + const_argument;
      }

      EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
    }
  }
}

HWTEST(TestCLVisaInjectionRtBasic, ConstraintsSingleInput) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const int seed = 0;

  auto dst = cs::generate_vector<int>(size, seed);

  const compute::context context = compute::system::default_context();

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_constraints_single_input.cl");

  compute::kernel kernel =
      program.create_kernel("test_constraints_single_input");
  kernel.set_args(out_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst), dst.data());
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size, 0);

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, ConstraintsMultipleInputs) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const int seed = 0;

  auto s1 = cs::generate_vector<int>(size, seed);
  auto s2 = cs::generate_vector<int>(size, s1[0]);
  auto dst = cs::generate_vector<int>(size, s2[0]);

  const compute::context context = compute::system::default_context();

  compute::buffer in1_buffer(context, cs::size_in_bytes(s1),
                             compute::memory_object::read_only |
                                 compute::memory_object::use_host_ptr,
                             s1.data());
  compute::buffer in2_buffer(context, cs::size_in_bytes(s2),
                             compute::memory_object::read_only |
                                 compute::memory_object::use_host_ptr,
                             s2.data());
  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  compute::program program = compute_samples::build_program(
      context,
      "test_cl_visa_injection_rt_basic_constraints_multiple_inputs.cl");

  compute::kernel kernel =
      program.create_kernel("test_constraints_multiple_inputs");
  kernel.set_args(out_buffer, in1_buffer, in2_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst), dst.data());
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = s2[i] - 0xa55a;
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, ConstraintsSingleOutput) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const int seed = 0;

  auto dst = cs::generate_vector<int>(size, seed);

  const compute::context context = compute::system::default_context();

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_constraints_single_output.cl");

  compute::kernel kernel =
      program.create_kernel("test_constraints_single_output");
  kernel.set_args(out_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst), dst.data());
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = -dst[i] - dst[i];
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, ConstraintsMultipleOutputs) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const int seed = 0;

  auto dst = cs::generate_vector<int>(size, seed);

  const compute::context context = compute::system::default_context();

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  compute::program program = compute_samples::build_program(
      context,
      "test_cl_visa_injection_rt_basic_constraints_multiple_outputs.cl");

  compute::kernel kernel =
      program.create_kernel("test_constraints_multiple_outputs");
  kernel.set_args(out_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst), dst.data());
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      int dst1 = i + 1, dst2 = i + 2, dst3 = i + 3;

      dst1 <<= 8;
      dst2 <<= 16;
      dst3 <<= 24;

      expected[i] = dst1 ^ dst2 ^ dst3;
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, ConstraintsLoadStorePointer) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const int seed = 0;

  auto src = cs::generate_vector<int>(size, seed);
  auto dst = cs::generate_vector<int>(size, src[0]);

  const compute::context context = compute::system::default_context();

  compute::buffer in_buffer(context, cs::size_in_bytes(src),
                            compute::memory_object::read_only |
                                compute::memory_object::use_host_ptr,
                            src.data());
  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  compute::program program = compute_samples::build_program(
      context,
      "test_cl_visa_injection_rt_basic_constraints_load_store_pointer.cl");

  compute::kernel kernel =
      program.create_kernel("test_constraints_load_store_pointer");
  kernel.set_args(out_buffer, in_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst), dst.data());
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      expected[i] = src[i] + i;
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

HWTEST(TestCLVisaInjectionRtBasic, ConstraintsFloat) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const int seed = 0;

  auto src = cs::generate_vector<int>(size, seed);
  auto dst = cs::generate_vector<int>(size, src[0]);

  const compute::context context = compute::system::default_context();

  compute::buffer in_buffer(context, cs::size_in_bytes(src),
                            compute::memory_object::read_only |
                                compute::memory_object::use_host_ptr,
                            src.data());
  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_constraints_float.cl");

  compute::kernel kernel = program.create_kernel("test_constraints_float");
  kernel.set_args(out_buffer, in_buffer);

  compute::command_queue queue = compute::system::default_queue();
  queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst), dst.data());
  queue.enqueue_1d_range_kernel(kernel, 0, size, 0);

  {
    std::vector<int> result(size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(result),
                              result.data());
    std::vector<int> expected(size);
    for (size_t i = 0; i < size; ++i) {
      auto sf = static_cast<float>(src[i]);
      sf = -sf;
      expected[i] = static_cast<int>(sf);
    }

    EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
  }
}

template <typename T>
compute::buffer create_input_buffer(const compute::context &context, T &data) {
  return compute::buffer(context, cs::size_in_bytes(data),
                         compute::memory_object::read_only |
                             compute::memory_object::use_host_ptr,
                         data.data());
}

HWTEST(TestCLVisaInjectionRtBasic, FunctionBody) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 64;
  const int seed = 0;

  auto dst1 = cs::generate_vector<uint32_t>(size, seed);
  auto dst2 = cs::generate_vector<uint32_t>(size, dst1[0]);
  auto s_x0 = cs::generate_vector<uint32_t>(size, dst2[0]);
  auto s_x1 = cs::generate_vector<uint32_t>(size, s_x0[0]);
  auto s_x2 = cs::generate_vector<uint32_t>(size, s_x1[0]);

  const compute::context context = compute::system::default_context();

  auto s_x0_buff = create_input_buffer(context, s_x0);
  auto s_x1_buff = create_input_buffer(context, s_x1);
  auto s_x2_buff = create_input_buffer(context, s_x2);

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_func_body.cl");
  compute::command_queue queue = compute::system::default_queue();

  auto run_kernel = [&](const char *kernel_name,
                        std::vector<uint32_t> &dst) -> void {
    compute::kernel kernel = program.create_kernel(kernel_name);
    kernel.set_args(out_buffer, s_x0_buff, s_x1_buff, s_x2_buff);
    queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst),
                               dst.data());
    queue.enqueue_1d_range_kernel(kernel, 0, size, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(dst),
                              dst.data());
  };

  run_kernel("test_call_func1", dst1);
  run_kernel("test_call_func2", dst2);

  EXPECT_THAT(dst1, ::testing::ElementsAreArray(dst2));
}

HWTEST(TestCLVisaInjectionRtBasic, PragmaUnroll) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t width = 64;
  const size_t height = 64;
  const size_t size = width * height;
  const int seed = 0;

  auto src_mx = cs::generate_vector<uint32_t>(size, seed);
  auto src_vec = cs::generate_vector<uint32_t>(width, src_mx[0]);
  auto dst1 = cs::generate_vector<uint32_t>(height, src_vec[0]);
  auto dst2 = cs::generate_vector<uint32_t>(height, dst1[0]);
  auto dst3 = cs::generate_vector<uint32_t>(height, dst2[0]);

  const compute::context context = compute::system::default_context();

  auto src_mx_buffer = create_input_buffer(context, src_mx);
  auto src_vec_buffer = create_input_buffer(context, src_vec);

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_pragma_unroll.cl");
  compute::command_queue queue = compute::system::default_queue();

  auto run_kernel = [&](const char *kernel_name,
                        std::vector<uint32_t> &dst) -> void {
    compute::kernel kernel = program.create_kernel(kernel_name);
    kernel.set_args(src_mx_buffer, src_vec_buffer, out_buffer);
    queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst),
                               dst.data());
    queue.enqueue_1d_range_kernel(kernel, 0, width, 0);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(dst),
                              dst.data());
  };

  run_kernel("test_kernel", dst1);

  run_kernel("test_kernel_unroll", dst2);
  EXPECT_THAT(dst1, ::testing::ElementsAreArray(dst2));

  run_kernel("test_kernel_unroll_with_asm", dst3);
  EXPECT_THAT(dst1, ::testing::ElementsAreArray(dst3));
}

HWTEST(TestCLVisaInjectionRtBasic, WholeKernel) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t width = 64;
  const size_t height = 64;
  const size_t size = width * height;
  const int seed = 0;

  auto dst1 = cs::generate_vector<uint32_t>(size, seed);
  auto dst2 = cs::generate_vector<uint32_t>(size, dst1[0]);
  auto src = cs::generate_vector<uint32_t>(size, dst2[0]);

  const compute::context context = compute::system::default_context();

  auto src_buffer = create_input_buffer(context, src);

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  auto run_kernel = [&](const char *file_name, const char *kernel_name,
                        std::vector<uint32_t> &dst) -> void {
    compute::program program =
        compute_samples::build_program(context, file_name);
    compute::command_queue queue = compute::system::default_queue();

    compute::kernel kernel = program.create_kernel(kernel_name);
    kernel.set_args(src_buffer, out_buffer);
    queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst),
                               dst.data());
    queue.enqueue_nd_range_kernel(kernel, 2, nullptr,
                                  compute::dim(width, height).data(), nullptr);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(dst),
                              dst.data());
  };

  run_kernel("test_cl_visa_injection_rt_basic_median_filter.cl",
             "median_filter", dst1);
  run_kernel("test_cl_visa_injection_rt_basic_whole_kernel.cl",
             "test_whole_kernel", dst2);

  EXPECT_THAT(dst1, ::testing::ElementsAreArray(dst2));
}

HWTEST(TestCLVisaInjectionRtBasic, SingleBasicBlock) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t width = 64;
  const size_t height = 64;
  const size_t size = width * height;
  const int seed = 0;

  auto dst1 = cs::generate_vector<uint32_t>(size, seed);
  auto dst2 = cs::generate_vector<uint32_t>(size, dst1[0]);
  auto src = cs::generate_vector<uint32_t>(size, dst2[0]);

  const compute::context context = compute::system::default_context();

  auto src_buffer = create_input_buffer(context, src);

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  auto run_kernel = [&](const char *file_name, const char *kernel_name,
                        std::vector<uint32_t> &dst) -> void {
    compute::program program =
        compute_samples::build_program(context, file_name);
    compute::command_queue queue = compute::system::default_queue();

    compute::kernel kernel = program.create_kernel(kernel_name);
    kernel.set_args(src_buffer, out_buffer);
    queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst),
                               dst.data());
    queue.enqueue_nd_range_kernel(kernel, 2, nullptr,
                                  compute::dim(width, height).data(), nullptr);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(dst),
                              dst.data());
  };

  run_kernel("test_cl_visa_injection_rt_basic_median_filter.cl",
             "median_filter", dst1);
  run_kernel("test_cl_visa_injection_rt_basic_single_basic_block.cl",
             "test_single_basic_block", dst2);

  EXPECT_THAT(dst1, ::testing::ElementsAreArray(dst2));
}

HWTEST(TestCLVisaInjectionRtBasic, Region) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t width = 64;
  const size_t height = 64;
  const size_t size = width * height;
  const int seed = 0;

  auto dst1 = cs::generate_vector<uint32_t>(size, seed);
  auto dst2 = cs::generate_vector<uint32_t>(size, dst1[0]);
  auto src = cs::generate_vector<uint32_t>(size, dst2[0]);

  const compute::context context = compute::system::default_context();

  auto src_buffer = create_input_buffer(context, src);

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  auto run_kernel = [&](const char *file_name, const char *kernel_name,
                        std::vector<uint32_t> &dst) -> void {
    compute::program program =
        compute_samples::build_program(context, file_name);
    compute::command_queue queue = compute::system::default_queue();

    compute::kernel kernel = program.create_kernel(kernel_name);
    kernel.set_args(src_buffer, out_buffer);
    queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst),
                               dst.data());
    queue.enqueue_nd_range_kernel(kernel, 2, nullptr,
                                  compute::dim(width, height).data(), nullptr);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(dst),
                              dst.data());
  };

  run_kernel("test_cl_visa_injection_rt_basic_median_filter.cl",
             "median_filter", dst1);
  run_kernel("test_cl_visa_injection_rt_basic_region.cl", "test_region", dst2);

  EXPECT_THAT(dst1, ::testing::ElementsAreArray(dst2));
}

HWTEST(TestCLVisaInjectionRtBasic, ForLoop) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t width = 64;
  const size_t height = 64;
  const size_t size = width * height;
  const int seed = 0;

  auto dst1 = cs::generate_vector<uint32_t>(size, seed);
  auto dst2 = cs::generate_vector<uint32_t>(size, dst1[0]);
  auto src = cs::generate_vector<uint32_t>(size, dst2[0]);

  const compute::context context = compute::system::default_context();

  auto src_buffer = create_input_buffer(context, src);

  compute::buffer out_buffer(context, size * 4,
                             compute::memory_object::read_write);

  auto run_kernel = [&](const char *file_name, const char *kernel_name,
                        std::vector<uint32_t> &dst) -> void {
    compute::program program =
        compute_samples::build_program(context, file_name);
    compute::command_queue queue = compute::system::default_queue();

    compute::kernel kernel = program.create_kernel(kernel_name);
    kernel.set_args(src_buffer, out_buffer);
    queue.enqueue_write_buffer(out_buffer, 0, cs::size_in_bytes(dst),
                               dst.data());
    queue.enqueue_nd_range_kernel(kernel, 2, nullptr,
                                  compute::dim(width, height).data(), nullptr);
    queue.enqueue_read_buffer(out_buffer, 0, cs::size_in_bytes(dst),
                              dst.data());
  };

  run_kernel("test_cl_visa_injection_rt_basic_median_filter.cl",
             "median_filter", dst1);
  run_kernel("test_cl_visa_injection_rt_basic_for_loop.cl", "test_for_loop",
             dst2);

  EXPECT_THAT(dst1, ::testing::ElementsAreArray(dst2));
}

HWTEST(TestCLVisaInjectionRtBasic, SGEMM) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t m = 16 * 16;
  const size_t n = 16 * 16;
  const size_t p = 16 * 16;
  const int seed = 0;

  auto a = cs::generate_vector<float>(m * n, 0, 128, seed);
  auto b = cs::generate_vector<float>(n * p, 0, 128, seed + 1);
  auto c = cs::generate_vector<float>(m * p, 0, 128, seed + 2);

  const float alpha = cs::generate_value(0, 128, seed + 3);
  const float beta = cs::generate_value(0, 128, seed + 4);

  const compute::context context = compute::system::default_context();

  auto a_buff = create_input_buffer(context, a);
  auto b_buff = create_input_buffer(context, b);

  compute::buffer c_buff(context, cs::size_in_bytes(c),
                         compute::memory_object::read_write);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_gemm.cl", "-DTYPE=float");
  compute::command_queue queue = compute::system::default_queue();

  auto run_kernel = [&](const char *kernel_name,
                        std::vector<float> &dst) -> void {
    compute::kernel kernel = program.create_kernel(kernel_name);
    kernel.set_args(a_buff, b_buff, c_buff, alpha, beta, n, m);
    queue.enqueue_write_buffer(c_buff, 0, cs::size_in_bytes(c), c.data());
    queue.enqueue_nd_range_kernel(kernel, 2, nullptr, compute::dim(m, p).data(),
                                  nullptr);
    queue.enqueue_read_buffer(c_buff, 0, cs::size_in_bytes(c), dst.data());
  };

  decltype(c) dst1(m * p);
  run_kernel("test_gemm", dst1);

  decltype(c) dst2(m * p);
  run_kernel("test_asm_sgemm", dst2);

  EXPECT_THAT(dst1, ::testing::Pointwise(::testing::FloatEq(), dst2));
}

HWTEST(TestCLVisaInjectionRtBasic, StencilFloat) {
  EXPECT_TRUE(check_supported_subgroup_size(16));

  const size_t size = 256;
  const int seed = 0;

  auto input = cs::generate_vector<float>(size * size, 0, 128, seed);
  auto output = cs::generate_vector<float>(size * size, 0, 128, seed + 1);

  const compute::context context = compute::system::default_context();

  auto input_buff = create_input_buffer(context, input);

  compute::buffer output_buff(context, cs::size_in_bytes(output),
                              compute::memory_object::read_write);

  compute::program program = compute_samples::build_program(
      context, "test_cl_visa_injection_rt_basic_stencil_5_point.cl",
      "-DTYPE=float");
  compute::command_queue queue = compute::system::default_queue();

  auto run_kernel = [&](const char *kernel_name,
                        std::vector<float> &dst) -> void {
    compute::kernel kernel = program.create_kernel(kernel_name);
    kernel.set_args(size, input_buff, output_buff);
    queue.enqueue_write_buffer(output_buff, 0, cs::size_in_bytes(output),
                               output.data());
    queue.enqueue_nd_range_kernel(kernel, 2, nullptr,
                                  compute::dim(size, size).data(), nullptr);
    queue.enqueue_read_buffer(output_buff, 0, cs::size_in_bytes(output),
                              dst.data());
  };

  decltype(output) dst1(size * size);
  run_kernel("test_stencil_5_point", dst1);

  decltype(output) dst2(size * size);
  run_kernel("test_asm_stencil_5_point", dst2);

  EXPECT_THAT(dst1, ::testing::Pointwise(::testing::FloatNear(0.00001f), dst2));
}

} // namespace
