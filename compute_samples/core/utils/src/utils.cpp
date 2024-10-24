/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "utils/utils.hpp"
#include "logging/logging.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace compute_samples {
std::string load_text_file(const std::string &file_path) {
  LOG_ENTER_FUNCTION
  LOG_DEBUG << "File path: " << file_path;

  std::ifstream stream(file_path.c_str());

  std::string text_file;
  if (!stream.good()) {
    LOG_ERROR << "Failed to load text file: " << file_path;
    LOG_EXIT_FUNCTION
    return text_file;
  }

  text_file = std::string((std::istreambuf_iterator<char>(stream)),
                          std::istreambuf_iterator<char>());
  LOG_DEBUG << "Text file loaded";

  LOG_EXIT_FUNCTION
  return text_file;
}

void save_text_file(const std::string data, const std::string &file_path) {
  LOG_ENTER_FUNCTION
  LOG_DEBUG << "File path: " << file_path;

  std::ofstream ofs(file_path);
  ofs << data;
  ofs.close();

  LOG_EXIT_FUNCTION
}

std::vector<uint8_t> load_binary_file(const std::string &file_path) {
  LOG_ENTER_FUNCTION
  LOG_DEBUG << "File path: " << file_path;
  std::ifstream stream(file_path, std::ios::in | std::ios::binary);

  std::vector<uint8_t> binary_file;
  if (!stream.good()) {
    LOG_ERROR << "Failed to load binary file: " << file_path;
    LOG_EXIT_FUNCTION
    return binary_file;
  }

  size_t length = 0;
  stream.seekg(0, std::ifstream::end);
  length = static_cast<size_t>(stream.tellg());
  stream.seekg(0, std::ifstream::beg);
  LOG_DEBUG << "Binary file length: " << length;

  binary_file.resize(length);
  stream.read(reinterpret_cast<char *>(binary_file.data()), length);
  LOG_DEBUG << "Binary file loaded";

  LOG_EXIT_FUNCTION
  return binary_file;
}

void save_binary_file(const std::vector<uint8_t> &data,
                      const std::string &file_path) {
  LOG_ENTER_FUNCTION
  LOG_DEBUG << "File path: " << file_path;

  std::ofstream stream(file_path, std::ios::out | std::ios::binary);
  stream.write(reinterpret_cast<const char *>(data.data()),
               size_in_bytes(data));

  LOG_EXIT_FUNCTION
}

std::string uuid_to_string(const uint8_t uuid[]) {
  std::stringstream ss;
  ss << std::hex << std::setfill('0') << std::uppercase;
  ss << std::setw(2) << +uuid[15];
  ss << std::setw(2) << +uuid[14];
  ss << std::setw(2) << +uuid[13];
  ss << std::setw(2) << +uuid[12];
  ss << '-';
  ss << std::setw(2) << +uuid[11];
  ss << std::setw(2) << +uuid[10];
  ss << '-';
  ss << std::setw(2) << +uuid[9];
  ss << std::setw(2) << +uuid[8];
  ss << '-';
  ss << std::setw(2) << +uuid[7];
  ss << std::setw(2) << +uuid[6];
  ss << '-';
  ss << std::setw(2) << +uuid[5];
  ss << std::setw(2) << +uuid[4];
  ss << std::setw(2) << +uuid[3];
  ss << std::setw(2) << +uuid[2];
  ss << std::setw(2) << +uuid[1];
  ss << std::setw(2) << +uuid[0];
  return ss.str();
}

void erase_substring(std::string &string, const std::string &substring) {
  if (substring.empty()) {
    return;
  }
  size_t pos = std::string::npos;
  while ((pos = string.find(substring)) != std::string::npos) {
    string.erase(pos, substring.length());
  }
}

void erase_substrings(std::string &string,
                      const std::vector<std::string> &substrings) {
  for (const auto &substring : substrings) {
    erase_substring(string, substring);
  }
}

std::vector<std::string> split_string(const std::string &string,
                                      const std::string &delimeter) {
  if (string.empty() || delimeter.empty()) {
    return {string};
  }
  std::vector<std::string> tokens;
  size_t start = 0U;
  size_t end = string.find(delimeter);
  while (end != std::string::npos) {
    tokens.push_back(string.substr(start, end - start));
    start = end + delimeter.length();
    end = string.find(delimeter, start);
  }
  tokens.push_back(string.substr(start, string.back()));
  return tokens;
}

std::string join_strings(const std::vector<std::string> &tokens,
                         const std::string &delimeter) {
  std::stringstream ss;
  for (size_t i = 0; i < tokens.size(); ++i) {
    ss << tokens[i];
    if (i < tokens.size() - 1) {
      ss << delimeter;
    }
  }
  return ss.str();
}

} // namespace compute_samples
