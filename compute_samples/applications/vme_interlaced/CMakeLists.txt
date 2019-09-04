#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(vme_interlaced
    SOURCE
    "include/vme_interlaced/vme_interlaced.hpp"
    "src/vme_interlaced.cpp"
)
target_link_libraries(vme_interlaced_lib
    PUBLIC
    compute_samples::timer
    compute_samples::logging
    Boost::program_options
    compute_samples::yuv_utils
    compute_samples::align_utils
    compute_samples::ocl_utils
)
add_kernels(vme_interlaced_lib "vme_interlaced.cl")

add_application(vme_interlaced
    SOURCE
    "src/main.cpp"
)
install_kernels(vme_interlaced "vme_interlaced.cl")
install_resources(vme_interlaced FILES "${MEDIA_DIRECTORY}/yuv/football_interlaced_720x480.yuv")

add_application_test(vme_interlaced
    SOURCE
    "test/main.cpp"
    "test/vme_interlaced_integration_tests.cpp"
    "test/vme_interlaced_system_tests.cpp"
)
install_kernels(vme_interlaced_tests "vme_interlaced.cl")
install_resources(vme_interlaced_tests FILES "${MEDIA_DIRECTORY}/yuv/football_interlaced_720x480.yuv")
