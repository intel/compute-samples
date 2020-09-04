# VISA injection run-time tests

## Overview

Basic run-time tests for OCL inline assembler.
All tests are self-checking, i.e. the host code should check results of execution of kernels under test on HW against reference values.

A documentation on how to inline Intel Virtual ISA (vISA) assembly language into user compute program code, and compiled into GenX binary using the Intel Graphics Compiler (IGC) is available at:
    https://github.com/intel/intel-graphics-compiler

The inline assembler will use the GNU extended-asm syntax:
https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
```
asm [qualifiers] ([Assembler Template]
    : Constraint (Output Operands)
    : Constraint (Input Operands));
```

**NB**
This feature is experimental and may not be supported on our drivers or with a limited support.

## Test cases examples

- Empty template
  example:
  ```
  __asm__("");
  ```
- Template without operands
  example:
  ```
  __asm__("barrier");
  ```
- Arbitrary order of positional operands
  example:
  ```
  __asm__("add (M1,16) %0(0,0)<1> %1(0,0)<1;1,0> (-)%0(0,0)<1;1,0>"
          : "+rw"(dst)
          : "rw"(src1));
  ```
- No input operands
  example:
  ```
  __asm__("mul (M1,16) %0(0,0)<1> %0(0,0)<1;1,0> %0(0,0)<1;1,0>" : "+rw"(dst));
  ```
- No output operands
  example:
  ```
  __asm__ ("add (M1, 16) %1(0,0)<1> %1(0,0)<1;1,0> %0"
           :
           : "i"(0x42), "rw"(tmp));
  ```
- Multiple vISA instructions
  example:
  ```
  __asm__ ("add (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> (-)%2(0,0)<1;1,0>\n"
           "div (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> %1(0,0)<1;1,0>"
           : "+rw"(dst)
           : "rw"(tmp2), "rw"(tmp1));
  ```
- Variable declaration
  example:
  ```
    __asm__ (
      ".decl tmp1 v_type=G type=d num_elts=16 align=GRF\n"
      "div (M1, 16) tmp1(0,0)<1> %1(0,0)<1;1,0> %2(0,0)<1;1,0>\n"
      "add (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> (-)tmp1(0,0)<1;1,0>\n"
      ".decl tmp2 v_type=G type=d num_elts=16 align=GRF alias=<tmp1, 0>\n"
      "add (M1, 16) tmp2(0,0)<1> %1(0,0)<1;1,0> (-)%2(0,0)<1;1,0>\n"
      "mul (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> tmp2(0,0)<1;1,0>"
      : "+rw"(dst)
      : "rw"(src2), "rw"(src1));
  ```
- Scopes
  example:
  ```
   __asm__ ("{\n"
            ".decl tmp v_type=G type=d num_elts=16 align=GRF\n"
            "mul (M1, 16) tmp(0,0)<1> %1(0,0)<1;1,0> %0(0,0)<1;1,0>\n"
            "add (M1, 16) %0(0,0)<1> tmp(0,0)<1;1,0> (-)%2(0,0)<1;1,0>\n"
            "}\n"
            : "+rw"(dst)
            : "rw"(src2), "rw"(src1));
  __asm__ ("{\n"
           ".decl tmp v_type=G type=d num_elts=16 align=GRF\n"
           "div (M1, 16) tmp(0,0)<1> %2(0,0)<1;1,0> %1(0,0)<1;1,0>\n"
           "add (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> tmp(0,0)<1;1,0>\n"
           "}\n"
           : "+rw"(dst)
           : "rw"(src2), "rw"(src1));
  ```
- Same input/output with "+"
  example:
  ```
  __asm__("mul (M1, 16) %0(0,0)<1> %1(0,0)<1;1,0> %0(0,0)<1;1,0>"
          : "+rw"(dst)
          : "rw"(dst));
  ```
- Same input/output with "="
  example:
  ```
  __asm__("add (M1, 16) %0(0,0)<1> %1(0,0)<1;1,0> %1(0,0)<1;1,0>"
          : "=rw"(dst)
          : "rw"(dst));
  ```
- "rw" with int/float/pointer types
  example:
  ```
  __asm__("mov (M1, 16) %0(0,0)<1> (-)%0(0,0)<1;1,0>" : "+rw"(sf));
  ```
- Single input
  example:
  ```
  __asm__("xor (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> %0(0,0)<1;1,0>"
          :
          : "+rw"(dst));
  ```
- Multiple inputs
  example:
  ```
  __asm__("add (M1, 16) %1(0,0)<1> %2(0,0)<1;1,0> %0"
          :
          : "i"(0xffff5aa6), "=rw"(dst), "rw"(src1), "rw"(src0));
  ```
- Single output
  example:
  ```
  __asm__("add (M1, 16) %0(0,0)<1> (-)%0(0,0)<1;1,0> (-)%0(0,0)<1;1,0>"
          : "+rw"(dst));
  ```
- Multiple outputs
  example:
  ```
  __asm__("shl (M1, 16) %2(0,0)<1> %2(0,0)<1;1,0> 0x8:d\n"
          "shl (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> 0x10:d\n"
          "shl (M1, 16) %1(0,0)<1> %1(0,0)<1;1,0> 0x18:d\n"
          : "+rw"(dst2), "+rw"(dst3), "+rw"(dst1));
  ```
- Immediate operands
  example:
  ```
  __asm__("or (M1, 16) %0(0,0)<1> %1 %0(0,0)<1;1,0>"
          : "+rw"(dst)
          : "i"(1 << (sizeof(int) * 8 - 1) | 0x42));
  ```
- Pointer Load/Store
  example:
  ```
  kernel(global int *const d, global const int *const s) {
      const size_t tid = get_global_id(0);
      int src;

      __asm__ ("svm_gather.4.1 (M1, 16) %1.0 %0.0"
              : "=rw"(src)
              : "rw"(&s[tid]));
  }
  ```


## Status

1. Basic coverage
    - [x] Template String Formatting cases
      - [x] Empty template
        - TemplateEmpty: [test_cl_visa_injection_rt_basic_template_empty.cl](test_cl_visa_injection_rt_basic_template_empty.cl)
      - [x] Template without operands
        - TemplateNoOperands: [test_cl_visa_injection_rt_basic_template_no_operands.cl](test_cl_visa_injection_rt_basic_template_no_operands.cl)
      - [x] Arbitrary order of positional operands
        - TemplateOperandsWithArbOrder: [test_cl_visa_injection_rt_basic_template_operands_ao.cl](test_cl_visa_injection_rt_basic_template_operands_ao.cl)
      - [x] No input operands
        - TemplateNoInputs: [test_cl_visa_injection_rt_basic_template_no_inputs.cl](test_cl_visa_injection_rt_basic_template_no_inputs.cl)
      - [x] No output operands
        - TemplateNoOutputs: [test_cl_visa_injection_rt_basic_template_no_outputs.cl](test_cl_visa_injection_rt_basic_template_no_outputs.cl)
      - [x] Multiple vISA instructions
        - TemplateMultipleIstructions: [test_cl_visa_injection_rt_basic_template_mltpl_insts.cl](test_cl_visa_injection_rt_basic_template_mltpl_insts.cl)
      - [x] Variable declaration
        - TemplateVariableDeclaration: [test_cl_visa_injection_rt_basic_template_var_decl.cl](test_cl_visa_injection_rt_basic_template_var_decl.cl)
      - [x] Scopes
        - TemplateVariableScope [test_cl_visa_injection_rt_basic_template_var_scope.cl](test_cl_visa_injection_rt_basic_template_var_scope.cl)
    - [x] Constraints cases
      - [x] Same input/output with "+"
        - ConstraintsSameInputOutputWithPlus: [test_cl_visa_injection_rt_basic_constraints_same_io.cl](test_cl_visa_injection_rt_basic_constraints_same_io.cl)
      - [x] Same input/output with "="
        - ConstraintsSameInputOutputWithEqual: [test_cl_visa_injection_rt_basic_constraints_same_io.cl](test_cl_visa_injection_rt_basic_constraints_same_io.cl)
      - [x] "rw" with int/float/pointer types
        - ConstraintsFloat: [test_cl_visa_injection_rt_basic_constraints_float.cl](test_cl_visa_injection_rt_basic_constraints_float.cl)
      - [x] Single input
        - ConstraintsSingleInput: [test_cl_visa_injection_rt_basic_constraints_single_input.cl](test_cl_visa_injection_rt_basic_constraints_single_input.cl)
      - [x] Multiple inputs
        - ConstraintsMultipleInputs: [test_cl_visa_injection_rt_basic_constraints_multiple_inputs.cl](test_cl_visa_injection_rt_basic_constraints_multiple_inputs.cl)
      - [x] Single output
        - ConstraintsSingleOutput: [test_cl_visa_injection_rt_basic_constraints_single_output.cl](test_cl_visa_injection_rt_basic_constraints_single_output.cl)
      - [x] Multiple outputs
        - ConstraintsMultipleOutputs: [test_cl_visa_injection_rt_basic_constraints_multiple_outputs.cl](test_cl_visa_injection_rt_basic_constraints_multiple_outputs.cl)
      - [x] Immediate operands
        - ConstraintsImmediateOperand: [test_cl_visa_injection_rt_basic_constraints_immediate.cl](test_cl_visa_injection_rt_basic_constraints_immediate.cl)
      - [x] Pointer Load/Store
        - ConstraintsLoadStorePointer: [test_cl_visa_injection_rt_basic_constraints_load_store_pointer.cl](test_cl_visa_injection_rt_basic_constraints_load_store_pointer.cl)
2. Extended coverage
   - [x] Comparison results of function written in OpenCL vs inline assembler
     - FunctionBody [test_cl_visa_injection_rt_basic_func_body.cl](test_cl_visa_injection_rt_basic_func_body.cl)
   - [x] Assembler in loops with loop unroll via pragma
     - PragmaUnroll [test_cl_visa_injection_rt_basic_pragma_unroll.cl](test_cl_visa_injection_rt_basic_pragma_unroll.cl)
   - [x] Replace the whole kernel bodies with their corresponding inline assembler
     - WholeKernel [test_cl_visa_injection_rt_basic_whole_kernel.cl](test_cl_visa_injection_rt_basic_whole_kernel.cl)
   - [x] Replace small sections of code (e.g., read/write or single BB) with their corresponding inline assembler
     - SingleBasicBlock [test_cl_visa_injection_rt_basic_single_basic_block.cl](test_cl_visa_injection_rt_basic_single_basic_block.cl)
   - [x] Replace a region of vISA code (region means with control flow) with its corresponding inline assembler
     - Region [test_cl_visa_injection_rt_basic_region.cl](test_cl_visa_injection_rt_basic_region.cl)
   - [x] Replace a for loop w/ its body with its corresponding inline assembler
     - ForLoop [test_cl_visa_injection_rt_basic_for_loop.cl](test_cl_visa_injection_rt_basic_for_loop.cl)
   - [x] Naive implementation GEMM in visa
     - SGEMM [test_cl_visa_injection_rt_basic_gemm.cl](test_cl_visa_injection_rt_basic_gemm.cl)
   - [x] 2D 5 points stencil operation in assembler
     - StensilFloat [test_cl_visa_injection_rt_basic_stencil_5_point.cl](test_cl_visa_injection_rt_basic_stencil_5_point.cl)

## Selecting tests

* To list all of available test cases please use `--gtest_list_tests` flag.
* To run only selected test case please use `--gtest_filter` flag.

More details can be found in [Google Test documentation]([https://github.com/google/googletest/blob/master/googletest/docs/advanced.md#selecting-tests).
