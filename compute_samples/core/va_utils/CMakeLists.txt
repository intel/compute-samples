#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

if(UNIX)
add_core_library(va_utils
    SOURCE
    "include/va_utils/va_utils.hpp"
    "src/va_utils.cpp"
)

include(import_libva)

target_link_libraries(va_utils
    PUBLIC
    VA::VA
    dl
    compute_samples::ocl_utils
)

endif()
