/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef COMPUTE_SAMPLES_USM_LINKED_LIST_HPP
#define COMPUTE_SAMPLES_USM_LINKED_LIST_HPP

#include <vector>

#include <boost/compute/core.hpp>
#include <boost/compute/intel/command_queue.hpp>
#include <boost/compute/intel/kernel.hpp>
namespace compute = boost::compute;

#include "application/application.hpp"
#include "ocl_utils/unified_shared_memory.hpp"

namespace compute_samples {

class UsmLinkedListApplication : public Application {
private:
  Status run_implementation(std::vector<std::string> &command_line) override;
  struct Arguments {
    bool help = false;
    compute::usm_type type = compute::usm_type::host;
    int size = 0;
  };
  Arguments
  parse_command_line(const std::vector<std::string> &command_line) const;
};

struct Node {
  Node *next = nullptr;
  cl_uint value = 0u;
};

template <typename T>
T *allocate_memory(const compute::usm_type type, const int size = 1) {
  compute::context context(compute::system::default_context());
  compute::device device(compute::system::default_device());

  T *p = nullptr;
  if (type == compute::usm_type::host) {
    p = compute::host_mem_alloc<T>(context, nullptr, size, 0);
  } else if (type == compute::usm_type::device) {
    p = compute::device_mem_alloc<T>(context, device, nullptr, size, 0);
  } else if (type == compute::usm_type::shared) {
    p = compute::shared_mem_alloc<T>(context, device, nullptr, size, 0);
  } else {
    throw std::runtime_error("Unknown USM type: " +
                             std::to_string(static_cast<int>(type)));
  }

  return p;
}

template <typename T> T read_memory(const T *p, const compute::usm_type type) {
  if (type == compute::usm_type::device) {
    T tmp;
    compute::command_queue_intel queue(compute::system::default_queue());
    queue.enqueue_memcpy(&tmp, p, sizeof(T));
    return tmp;
  }
  return *p;
}

compute::kernel_intel prepare_kernel(const compute::usm_type type);

Node *allocate_linked_list(const int size, const compute::usm_type type);
void walk_linked_list(Node *head, compute::kernel_intel &kernel);
void free_linked_list(Node *head);

} // namespace compute_samples

#endif
