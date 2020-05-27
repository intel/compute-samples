/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "gtest/gtest.h"
#include "usm_linked_list/usm_linked_list.hpp"
#include "ocl_utils/ocl_utils.hpp"
#include "test_harness/test_harness.hpp"

namespace cs = compute_samples;
namespace compute = boost::compute;

class UsmLinkedListIntegrationTests
    : public ::testing::TestWithParam<compute::usm_type> {};

HWTEST(UsmLinkedListIntegrationTests, ProgramCanBeBuilt) {
  const compute::device device = compute::system::default_device();
  const compute::context context(device);
  EXPECT_NE(compute::program(),
            compute_samples::build_program(context, "usm_linked_list.cl"));
}

HWTEST_P(UsmLinkedListIntegrationTests, PrepareKernel) {
  EXPECT_NO_THROW(cs::prepare_kernel(GetParam()));
}

HWTEST_P(UsmLinkedListIntegrationTests, AllocateMemory) {
  auto *p = cs::allocate_memory<cs::Node>(GetParam());
  EXPECT_NE(nullptr, p);

  compute::context context(compute::system::default_context());
  compute::mem_free(context, p);
}

HWTEST_P(UsmLinkedListIntegrationTests, AllocateLinkedList) {
  const int size = 10;
  cs::Node *head = cs::allocate_linked_list(size, GetParam());

  int actual = 0;
  cs::Node *current = head;
  while (current != nullptr) {
    actual++;
    if (GetParam() == compute::usm_type::device) {
      cs::Node host_copy_of_current;
      compute::command_queue_intel queue(compute::system::default_queue());
      queue.enqueue_memcpy(&host_copy_of_current, current, sizeof(cs::Node));
      current = host_copy_of_current.next;
    } else {
      current = current->next;
    }
  }

  EXPECT_EQ(size, actual);

  cs::free_linked_list(head, GetParam());
}

HWTEST_P(UsmLinkedListIntegrationTests, FreeLinkedList) {
  const int size = 10;
  cs::Node *head = cs::allocate_linked_list(size, GetParam());
  EXPECT_NO_THROW(cs::free_linked_list(head, GetParam()));
}

HWTEST_P(UsmLinkedListIntegrationTests, WalkLinkedList) {
  const int size = 10;
  cs::Node *head = cs::allocate_linked_list(size, GetParam());
  compute::kernel_intel kernel = cs::prepare_kernel(GetParam());

  cs::walk_linked_list(head, kernel);

  cs::Node *current = head;
  int count = 0;
  while (current != nullptr) {
    EXPECT_EQ(count * 4 + 1, cs::read_memory(current, GetParam()).value);
    if (GetParam() == compute::usm_type::device) {
      cs::Node host_copy_of_current;
      compute::command_queue_intel queue(compute::system::default_queue());
      queue.enqueue_memcpy(&host_copy_of_current, current, sizeof(cs::Node));
      current = host_copy_of_current.next;
    } else {
      current = current->next;
    }
    count++;
  }

  cs::free_linked_list(head, GetParam());
}

INSTANTIATE_TEST_SUITE_P(UsmTypes, UsmLinkedListIntegrationTests,
                         ::testing::Values(compute::usm_type::host,
                                           compute::usm_type::device,
                                           compute::usm_type::shared));
