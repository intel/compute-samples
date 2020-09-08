/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test_cl_visa_injection_rt_basic/test_cl_visa_injection_rt.hpp"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "ocl_utils/ocl_utils.hpp"

#include <boost/compute/core.hpp>
namespace compute = boost::compute;
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "logging/logging.hpp"
#include "utils/utils.hpp"
#include "test_harness/test_harness.hpp"
#include "random/random.hpp"
namespace cs = compute_samples;

namespace {

template <typename... Ts>
std::string make_simple_asm_kernel(const std::string &name,
                                   const std::string &body, Ts... tp) {
  std::string arg_types[sizeof...(Ts)] = {tp...};
  std::ostringstream os;
  os << "__attribute__((intel_reqd_sub_group_size(16)))\n"
     << "kernel void " << name << "() {\n"
     << "  __asm__(\"{\\n\"\n";

  for (unsigned i = 0; i < sizeof...(Ts); ++i) {
    os << "    \".decl tmp" << (i + 1) << " v_type=G type=" << arg_types[i]
       << " num_elts=16 align=GRF\\n\"\n";
  }

  os << "\"" << body << "\\n\"\n"
     << "  \"}\\n\"\n"
     << "  );\n"
     << "}\n";

  return os.str();
}

template <>
std::string make_simple_asm_kernel(const std::string &name,
                                   const std::string &body) {
  return make_simple_asm_kernel(name, body, "d", "d");
}

template <typename... Ts>
std::string check_asm_text_rt(const std::string &txt, Ts... tp) {
  const std::string kern_name = "my_kernel";
  const auto source = make_simple_asm_kernel(kern_name, txt, tp...);
  const char *source_str = source.c_str();

  const compute::context context = compute::system::default_context();

  cl_int ret = CL_SUCCESS;
  cl_program program =
      clCreateProgramWithSource(context, 1, &source_str, nullptr, &ret);
  EXPECT_TRUE(ret == CL_SUCCESS);

  ret = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
  EXPECT_FALSE(ret == CL_SUCCESS);

  auto device = context.get_device();

  size_t len = 0;
  ret = clGetProgramBuildInfo(program, device.id(), CL_PROGRAM_BUILD_LOG, 0,
                              nullptr, &len);
  EXPECT_TRUE(ret == CL_SUCCESS);

  char *buffer = new char[len];
  ret = clGetProgramBuildInfo(program, device.id(), CL_PROGRAM_BUILD_LOG, len,
                              buffer, nullptr);
  EXPECT_TRUE(ret == CL_SUCCESS);

  std::string build_log = buffer;
  delete[] buffer;

  LOG_DEBUG << "[check_asm_text_ocloc] build_log:\n" << build_log;

  return build_log;
}

template <typename... Ts>
std::string check_asm_text_ocloc(bool check_ret, const std::string &file_name,
                                 const std::string &txt,
                                 const std::string &device, Ts... tp) {
  const std::string cl_path = file_name + ".cl";
  const auto source = make_simple_asm_kernel("my_kernel", txt, tp...);
  LOG_DEBUG << "[check_asm_text_ocloc] kernel:\n" << source << std::endl;
  cs::save_text_file(source, cl_path);

  const std::string log_path = file_name + "_command_log.txt";
  std::string command_line = "ocloc -64 -file " + cl_path + " -device " +
                             device + " -output_no_suffix -output " + file_name;
  command_line += " > " + log_path + " 2>&1";
  LOG_DEBUG << "[check_asm_text_ocloc] cmd: " << command_line;

  const int return_code = std::system(command_line.c_str());
  const std::string log = cs::load_text_file(log_path);

  LOG_DEBUG << "[check_asm_text_ocloc] cmd output:\n" << log << std::endl;

  fs::remove(log_path.c_str());
  fs::remove(cl_path.c_str());

  if (check_ret) {
    EXPECT_FALSE(return_code == 0);
  }

  // cleanup
  for (auto &ext : {"", ".spv", ".gen"}) {
    const std::string path = file_name + ext;
    if (fs::exists(path)) {
      std::remove(path.c_str());
    }
  }

  return log;
}

inline bool contains(const std::string &str, const std::string &what) {
  return str.find(what) != std::string::npos;
}

const std::string file_prefix = "visa_injection_rt_negative_cases";

HWTEST(TestCLVisaInjectionRtNegativeCases, Simple) {
  const std::string asm_txt = "@@";

  std::string log = check_asm_text_rt(asm_txt);
  EXPECT_TRUE(contains(log, "syntax error"));

  log = check_asm_text_ocloc(true, file_prefix + "_simple", asm_txt, "skl");
  EXPECT_TRUE(contains(log, "syntax error"));
}

HWTEST(TestCLVisaInjectionRtNegativeCases, IllegalExecSize) {
  const std::string asm_txt = "mov (M1_NM, 6) tmp1(0,1)<1>  tmp2(0,0)<1;1,0>";

  std::string log = check_asm_text_rt(asm_txt);
  EXPECT_TRUE(contains(log, "invalid execution size"));

  log = check_asm_text_ocloc(true, file_prefix + "_illegal_exec_size", asm_txt,
                             "skl");
  EXPECT_TRUE(contains(log, "invalid execution size"));
}

HWTEST(TestCLVisaInjectionRtNegativeCases, MissingRegion) {
  const std::string asm_txt = "mov (M1_NM, 8) tmp1(0,1)<1>  tmp2(0,0)";

  std::string log = check_asm_text_rt(asm_txt);
  EXPECT_TRUE(contains(log, "syntax error"));

  log = check_asm_text_ocloc(true, file_prefix + "_missing_region", asm_txt,
                             "skl");
  EXPECT_TRUE(contains(log, "syntax error"));
}

HWTEST(TestCLVisaInjectionRtNegativeCases, BadOpcode) {
  const std::string asm_txt = "movi (M1_NM, 8) tmp1(0,1)<1>  tmp2(0,0)";

  std::string log = check_asm_text_rt(asm_txt);
  EXPECT_TRUE(contains(log, "syntax error"));

  log = check_asm_text_ocloc(true, file_prefix + "_bad_opcode", asm_txt, "skl");
  EXPECT_TRUE(contains(log, "syntax error"));
}

HWTEST(TestCLVisaInjectionRtNegativeCases, UndefinedDecl) {
  const std::string asm_txt = "mov (M1_NM, 8) tmp1(0,1)<1>  my_super_var(0,0)";

  std::string log = check_asm_text_rt(asm_txt);
  EXPECT_TRUE(contains(log, "syntax error"));

  log = check_asm_text_ocloc(true, file_prefix + "_undefined_decl", asm_txt,
                             "skl");
  EXPECT_TRUE(contains(log, "syntax error"));
}

HWTEST(TestCLVisaInjectionRtNegativeCases, UndefinedPred) {
  const std::string asm_txt = "cmp.lt (M1_NM, 8) P3 tmp1(0,0)<0;1,0> 0x3:ud";

  std::string log = check_asm_text_rt(asm_txt);
  EXPECT_TRUE(contains(log, "undefined predicate variable"));

  log = check_asm_text_ocloc(true, file_prefix + "_undefined_pred", asm_txt,
                             "skl");
  EXPECT_TRUE(contains(log, "undefined predicate variable"));
}

HWTEST(TestCLVisaInjectionRtNegativeCases, BadOperandSyntax) {
  const std::string asm_txt = "mov (M1_NM, 8) tmp1(0,1)<1>:f tmp2(0,0)<1;1,0>";

  std::string log = check_asm_text_rt(asm_txt);
  EXPECT_TRUE(contains(log, "syntax error"));

  log = check_asm_text_ocloc(true, file_prefix + "_bad_operand_syntax", asm_txt,
                             "skl");
  EXPECT_TRUE(contains(log, "syntax error"));
}

HWTEST(TestCLVisaInjectionRtNegativeCases, WrongDeclare) {
  const std::string asm_txt = ".decl TMP v_type=G type=f";

  std::string log = check_asm_text_rt(asm_txt);
  EXPECT_TRUE(contains(log, "syntax error"));

  log = check_asm_text_ocloc(true, file_prefix + "_wrong_declare", asm_txt,
                             "skl");
  EXPECT_TRUE(contains(log, "syntax error"));
}

HWTEST(TestCLVisaInjectionRtNegativeCases, MissingLabel) {
  const std::string asm_txt = "goto (M1, 16) check_label0";

  std::string log = check_asm_text_rt(asm_txt);
  EXPECT_TRUE(contains(log, "Found a total of 1 errors in vISA input"));

  log = check_asm_text_ocloc(true, file_prefix + "_missing_label", asm_txt,
                             "skl");
  EXPECT_TRUE(contains(log, "Found a total of 1 errors in vISA input"));
}

HWTEST(TestCLVisaInjectionRtNegativeCases, DuplicateLabel) {
  const std::string asm_txt = "check_label0:\\ncheck_label0:";

  std::string log = check_asm_text_rt(asm_txt);
  EXPECT_TRUE(contains(log, "Found a total of 1 errors in vISA input"));

  log = check_asm_text_ocloc(true, file_prefix + "_duplicate_label", asm_txt,
                             "skl");
  EXPECT_TRUE(contains(log, "Found a total of 1 errors in vISA input"));
}

HWTEST(TestCLVisaInjectionRtNegativeCases, UnsupportedInstructionDIVM) {
  const std::string asm_txt =
      "divm (M1, 8) tmp1(0,0)<1> tmp2(0,0)<1;1,0> 0x2:f";
  auto log = check_asm_text_ocloc(
      false, file_prefix + "_unsupported_instruction_divm_skl", asm_txt, "skl",
      "f", "f");
  EXPECT_TRUE(contains(log, "Build succeeded"));

  log = check_asm_text_ocloc(
      true, file_prefix + "_unsupported_instruction_divm_icllp", asm_txt,
      "icllp", "f", "f");
  EXPECT_FALSE(contains(log, "Build succeeded"));
}

HWTEST(TestCLVisaInjectionRtNegativeCases, UnsupportedInstructionROL) {
  const std::string asm_txt = "rol (M1, 8) tmp1(0,0)<1> tmp2(0,0)<1;1,0> 0x2:d";
  auto log = check_asm_text_ocloc(
      false, file_prefix + "_unsupported_instruction_rol_tgllp", asm_txt,
      "tgllp");
  EXPECT_TRUE(contains(log, "Build succeeded"));

  log = check_asm_text_ocloc(
      true, file_prefix + "_unsupported_instruction_rol_skl", asm_txt, "skl");
  EXPECT_FALSE(contains(log, "Build succeeded"));
}

} // namespace
