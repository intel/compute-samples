#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_test_suite(test_cl_visa_injection_rt_basic
  SOURCE
  "src/test_cl_visa_injection_rt_basic.cpp"
  "src/main.cpp")

target_link_libraries(test_cl_visa_injection_rt_basic
  PUBLIC
  compute_samples::logging
  compute_samples::ocl_utils
  compute_samples::utils
  compute_samples::random)

set(KERNEL_LIST
  "test_cl_visa_injection_rt_basic_template_empty.cl"
  "test_cl_visa_injection_rt_basic_template_no_operands.cl"
  "test_cl_visa_injection_rt_basic_template_operands_ao.cl"
  "test_cl_visa_injection_rt_basic_template_no_inputs.cl"
  "test_cl_visa_injection_rt_basic_template_no_outputs.cl"
  "test_cl_visa_injection_rt_basic_template_mltpl_insts.cl"
  "test_cl_visa_injection_rt_basic_template_var_decl.cl"
  "test_cl_visa_injection_rt_basic_template_var_scope.cl"
  "test_cl_visa_injection_rt_basic_constraints_same_io.cl"
  "test_cl_visa_injection_rt_basic_constraints_immediate.cl"
  "test_cl_visa_injection_rt_basic_constraints_single_input.cl"
  "test_cl_visa_injection_rt_basic_constraints_multiple_inputs.cl"
  "test_cl_visa_injection_rt_basic_constraints_single_output.cl"
  "test_cl_visa_injection_rt_basic_constraints_multiple_outputs.cl"
  "test_cl_visa_injection_rt_basic_constraints_load_store_pointer.cl"
  "test_cl_visa_injection_rt_basic_constraints_float.cl"
  "test_cl_visa_injection_rt_basic_func_body.cl"
  )

add_kernels(test_cl_visa_injection_rt_basic ${KERNEL_LIST})

install_kernels(test_cl_visa_injection_rt_basic ${KERNEL_LIST})
