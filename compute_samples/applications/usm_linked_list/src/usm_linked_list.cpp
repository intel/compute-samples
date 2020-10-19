/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "usm_linked_list/usm_linked_list.hpp"
#include "utils/utils.hpp"

#include <iostream>
#include <numeric>

#include <boost/program_options.hpp>

#include <boost/compute/core.hpp>
#include <boost/compute/intel/command_queue.hpp>
#include <boost/compute/intel/device.hpp>
#include "ocl_utils/ocl_utils.hpp"
#include "logging/logging.hpp"
#include "timer/timer.hpp"

namespace po = boost::program_options;
namespace compute = boost::compute;

namespace compute_samples {
Application::Status UsmLinkedListApplication::run_implementation(
    std::vector<std::string> &command_line) {
  const Arguments args = parse_command_line(command_line);
  if (args.help) {
    return Status::SKIP;
  }

  const compute::device_intel device(compute::system::default_device());
  LOG_INFO << "OpenCL device: " << device.name();

  if (!device.supports_extension("cl_intel_unified_shared_memory_preview")) {
    LOG_ERROR << "cl_intel_unified_shared_memory_preview extension is required";
    return Status::SKIP;
  }

  cl_device_unified_shared_memory_capabilities_intel capabilities = 0;
  if (args.type == compute::usm_type::host) {
    capabilities = device.host_mem_capabilities();
  } else if (args.type == compute::usm_type::device) {
    capabilities = device.device_mem_capabilities();
  } else if (args.type == compute::usm_type::shared) {
    capabilities = device.single_device_shared_mem_capabilities();
  }

  if ((capabilities & CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL) == 0u) {
    LOG_ERROR << "CL_UNIFIED_SHARED_MEMORY_ACCESS_INTEL capability is required";
    return Status::SKIP;
  }

  LOG_INFO << "Linked list size: " << args.size;
  LOG_INFO << "Unified shared memory type: " << args.type;

  Timer timer_total;
  Timer timer;

  Node *head = allocate_linked_list(args.size, args.type);
  timer.print("Linked list allocated");

  compute::kernel_intel kernel = prepare_kernel(args.type);
  timer.print("Kernel prepared");

  walk_linked_list(head, kernel);
  timer.print("Linked list traversed");

  free_linked_list(head, args.type);
  timer.print("Linked list freed");

  timer_total.print("Total");

  return Status::OK;
}

UsmLinkedListApplication::Arguments
UsmLinkedListApplication::parse_command_line(
    const std::vector<std::string> &command_line) const {
  Arguments args;

  po::options_description desc("Allowed options");
  auto options = desc.add_options();
  options("help,h", "show help message");
  options("type", po::value<compute::usm_type>(&args.type)->required(),
          "unified shared memory type: host, device or shared");
  options("size", po::value<int>(&args.size)->default_value(4),
          "number of list elements");

  po::positional_options_description p;
  p.add("type", 1);

  po::variables_map vm;
  po::store(
      po::command_line_parser(command_line).options(desc).positional(p).run(),
      vm);

  if (vm.count("help") != 0u) {
    std::cout << desc;
    args.help = true;
    return args;
  }

  po::notify(vm);
  return args;
}

compute::kernel_intel prepare_kernel(const compute::usm_type type) {
  compute::context context(compute::system::default_context());
  compute::program program = build_program(context, "usm_linked_list.cl");
  compute::kernel_intel kernel(program.create_kernel("usm_linked_list_kernel"));

  const cl_bool value = CL_TRUE;
  if (type == compute::usm_type::host) {
    kernel.set_exec_info(CL_KERNEL_EXEC_INFO_INDIRECT_HOST_ACCESS_INTEL,
                         sizeof(value), &value);
  } else if (type == compute::usm_type::device) {
    kernel.set_exec_info(CL_KERNEL_EXEC_INFO_INDIRECT_DEVICE_ACCESS_INTEL,
                         sizeof(value), &value);
  } else if (type == compute::usm_type::shared) {
    kernel.set_exec_info(CL_KERNEL_EXEC_INFO_INDIRECT_SHARED_ACCESS_INTEL,
                         sizeof(value), &value);
  } else {
    throw std::runtime_error("Unknown USM type: " +
                             std::to_string(static_cast<int>(type)));
  }

  return kernel;
}

Node *allocate_linked_list(const int size, const compute::usm_type type) {
  Node *head = nullptr;
  Node *current = nullptr;

  Node host_node;
  Node *device_current = nullptr;

  for (int i = 0; i < size; ++i) {
    if (i == 0) {
      head = allocate_memory<Node>(type);

      if (type == compute::usm_type::device) {
        device_current = head;
        current = &host_node;
      } else {
        current = head;
      }
    }

    current->value = i * 2;

    if (i != size - 1) {
      current->next = allocate_memory<Node>(type);
    } else {
      current->next = nullptr;
    }

    if (type == compute::usm_type::device) {
      compute::command_queue_intel queue(compute::system::default_queue());
      queue.enqueue_memcpy(device_current, current, sizeof(Node));
      device_current = current->next;
    } else {
      current = current->next;
    }
  }

  return head;
}

void walk_linked_list(Node *head, compute::kernel_intel &kernel) {
  kernel.set_arg_mem_ptr(0, head);

  compute::command_queue queue(compute::system::default_queue());
  queue.enqueue_1d_range_kernel(kernel, 0, 1, 0);
  queue.finish();
}

void free_linked_list(Node *head, const compute::usm_type type) {
  compute::context context(compute::system::default_context());
  Node *current = head;
  Node *next = nullptr;
  while (current != nullptr) {
    if (type == compute::usm_type::device) {
      Node host_copy_of_current;
      compute::command_queue_intel queue(compute::system::default_queue());
      queue.enqueue_memcpy(&host_copy_of_current, current, sizeof(Node));
      next = host_copy_of_current.next;
    } else {
      next = current->next;
    }
    compute::mem_free(context, current);
    current = next;
  }
}

} // namespace compute_samples
