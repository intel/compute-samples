#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(subgroups_imagecopy
    SOURCE
    "include/subgroups_imagecopy/subgroups_imagecopy.hpp"
    "src/subgroups_imagecopy.cpp"
)
target_link_libraries(subgroups_imagecopy_lib
    PUBLIC
    compute_samples::timer
    compute_samples::logging
    Boost::program_options
    compute_samples::image
    compute_samples::ocl_utils
)
add_kernels(subgroups_imagecopy_lib
    "subgroups_imagecopy_kernel.cl"
    "subgroups_imagecopy_solution.cl"
)

add_application(subgroups_imagecopy
    SOURCE
    "src/main.cpp"
)
install_kernels(subgroups_imagecopy
    "subgroups_imagecopy_kernel.cl"
    "subgroups_imagecopy_solution.cl"
)
install_resources(subgroups_imagecopy FILES "${MEDIA_DIRECTORY}/bmp/input.bmp")

add_application_test(subgroups_imagecopy
    SOURCE
    "test/main.cpp"
    "test/subgroups_imagecopy_integration_tests.cpp"
    "test/subgroups_imagecopy_system_tests.cpp"
)
install_kernels(subgroups_imagecopy_tests
    "subgroups_imagecopy_kernel.cl"
    "subgroups_imagecopy_solution.cl"
)
install_resources(subgroups_imagecopy_tests FILES "${MEDIA_DIRECTORY}/bmp/input.bmp")
