/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cstdlib>
#include <fstream>
#include "logging/logging.hpp"
#include "utils/utils.hpp"

namespace compute_samples {

std::string generate_spirv(const std::string &path,
                           const std::string &build_options) {
  const std::string log_file = "command_log.txt";
  const std::string spv_file = "kernel";

  std::string command_line = "ocloc -file " + path +
                             " -device skl -output_no_suffix -output " +
                             spv_file;
  if (!build_options.empty()) {
    command_line += " -options \"" + build_options + "\"";
  }
  command_line += " > " + log_file + " 2>&1";
  LOG_DEBUG << "Offline compiler command: " << command_line;

  const int return_code = std::system(command_line.c_str());
  const std::string log = load_text_file(log_file);
  if (std::remove(log_file.c_str()) != 0) {
    LOG_DEBUG << "Deleting file " << log_file.c_str() << " failed";
  }

  if (return_code != 0) {
    LOG_ERROR << "Offline compiler return code: " << return_code;
    LOG_ERROR << "Offline compiler log:\n" << log;
    throw std::runtime_error("Offline compiler failed");
  }
  LOG_DEBUG << "Offline compiler return code: " << return_code;
  LOG_DEBUG << "Offline compiler log:\n" << log;

  return spv_file + ".spv";
}

std::vector<uint8_t>
generate_spirv_from_source(const std::string &source,
                           const std::string &build_options) {
  LOG_DEBUG << "Source code passed to offline compiler: " << source;

  const std::string cl_path = "kernel.cl";
  save_text_file(source, cl_path);

  const std::string spv_path = generate_spirv(cl_path, build_options);
  const std::vector<uint8_t> spirv = load_binary_file(spv_path);

  if (std::remove(cl_path.c_str()) != 0) {
    LOG_DEBUG << "Deleting file " << cl_path.c_str() << " failed";
  }
  if (std::remove(spv_path.c_str()) != 0) {
    LOG_DEBUG << "Deleting file " << spv_path.c_str() << " failed";
  }

  return spirv;
}

} // namespace compute_samples
