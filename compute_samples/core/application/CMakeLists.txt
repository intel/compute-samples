#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_core_library(application
    SOURCE
    "include/application/application.hpp"
    "src/application.cpp"
)
target_link_libraries(application
    PUBLIC
    compute_samples::logging
    compute_samples::version
)
