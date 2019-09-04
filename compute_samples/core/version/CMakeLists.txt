#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

configure_file("src/version_config.cpp.in" "${PROJECT_BINARY_DIR}/generated/version_config.cpp")
add_core_library(version
    SOURCE
    "include/version/version.hpp"
    "src/version.cpp"
    "src/version_config.hpp"
    "${PROJECT_BINARY_DIR}/generated/version_config.cpp"
)
target_include_directories(version
    PRIVATE
    src
)
