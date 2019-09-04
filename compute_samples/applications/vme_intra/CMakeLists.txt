#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(vme_intra
    SOURCE
    "include/vme_intra/vme_intra.hpp"
    "src/vme_intra.cpp"
)
target_link_libraries(vme_intra_lib
    PUBLIC
    compute_samples::timer
    compute_samples::logging
    Boost::program_options
    compute_samples::yuv_utils
    compute_samples::align_utils
    compute_samples::ocl_utils
)
add_kernels(vme_intra_lib
    "vme_intra_0_tier.cl"
    "vme_intra.cl"
)

add_application(vme_intra
    SOURCE
    "src/main.cpp"
)
install_kernels(vme_intra
    "vme_intra_0_tier.cl"
    "vme_intra.cl"
    "../vme_hme/downsample_3_tier.cl"
    "../vme_hme/vme_streaminout.cl"
    "../vme_hme/vme_hme_n_tier.cl"
)
install_resources(vme_intra FILES "${MEDIA_DIRECTORY}/yuv/goal_1280x720.yuv")

add_application_test(vme_intra
    SOURCE
    "test/main.cpp"
    "test/vme_intra_integration_tests.cpp"
    "test/vme_intra_system_tests.cpp"
)
install_kernels(vme_intra_tests
    "vme_intra_0_tier.cl"
    "vme_intra.cl"
    "../vme_hme/downsample_3_tier.cl"
    "../vme_hme/vme_streaminout.cl"
    "../vme_hme/vme_hme_n_tier.cl"
)
install_resources(vme_intra_tests
    FILES
    "${MEDIA_DIRECTORY}/yuv/foreman_176x144.yuv"
    "${MEDIA_DIRECTORY}/yuv/intra_foreman_176x144.yuv"
)
