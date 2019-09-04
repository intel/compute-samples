#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_application_library(subgroups_visualization
    SOURCE
    "include/subgroups_visualization/subgroups_visualization.hpp"
    "src/subgroups_visualization.cpp"
)
target_link_libraries(subgroups_visualization_lib
    PUBLIC
    compute_samples::timer
    compute_samples::logging
    Boost::program_options
    compute_samples::image
    compute_samples::ocl_utils
)
add_kernels(subgroups_visualization_lib
    "subgroups_visualization_kernel.cl"
    "subgroups_visualization_solution.cl"
)

add_application(subgroups_visualization
    SOURCE
    "src/main.cpp"
)
install_kernels(subgroups_visualization
    "subgroups_visualization_kernel.cl"
    "subgroups_visualization_solution.cl"
)

add_application_test(subgroups_visualization
    SOURCE
    "test/main.cpp"
    "test/subgroups_visualization_integration_tests.cpp"
    "test/subgroups_visualization_system_tests.cpp"
)
install_kernels(subgroups_visualization_tests
    "subgroups_visualization_kernel.cl"
    "subgroups_visualization_solution.cl"
)
install_resources(subgroups_visualization_tests FILES "${MEDIA_DIRECTORY}/bmp/test_subgroups_visualization_reference.bmp")
