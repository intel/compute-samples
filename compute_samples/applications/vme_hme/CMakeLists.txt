#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(vme_hme
    SOURCE
    "include/vme_hme/vme_hme.hpp"
    "src/vme_hme.cpp"
)
target_link_libraries(vme_hme_lib
    PUBLIC
    compute_samples::timer
    compute_samples::logging
    Boost::program_options
    compute_samples::yuv_utils
    compute_samples::align_utils
    compute_samples::ocl_utils
)
add_kernels(vme_hme_lib
    "downsample_3_tier.cl"
    "vme_streaminout.cl"
    "vme_hme_n_tier.cl"
    "vme_hme_0_tier.cl"
    "vme_hme.cl"
)

add_application(vme_hme
    SOURCE
    "src/main.cpp"
)
install_kernels(vme_hme
    "downsample_3_tier.cl"
    "vme_streaminout.cl"
    "vme_hme_n_tier.cl"
    "vme_hme_0_tier.cl"
    "vme_hme.cl"
)
install_resources(vme_hme FILES "${MEDIA_DIRECTORY}/yuv/goal_1280x720.yuv")

add_application_test(vme_hme
    SOURCE
    "test/main.cpp"
    "test/vme_hme_integration_tests.cpp"
    "test/vme_hme_system_tests.cpp"
)
install_kernels(vme_hme_tests
    "downsample_3_tier.cl"
    "vme_streaminout.cl"
    "vme_hme_n_tier.cl"
    "vme_hme_0_tier.cl"
    "vme_hme.cl"
)
install_resources(vme_hme_tests
    FILES
    "${MEDIA_DIRECTORY}/yuv/goal_1280x720.yuv"
    "${MEDIA_DIRECTORY}/yuv/foreman_176x144.yuv"
    "${MEDIA_DIRECTORY}/yuv/hme_foreman_176x144.yuv"
)
