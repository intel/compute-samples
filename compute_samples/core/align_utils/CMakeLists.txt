#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_library(align_utils INTERFACE)
add_library(compute_samples::align_utils ALIAS align_utils)
target_include_directories(align_utils
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS align_utils
    EXPORT align_utils-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/align_utils DESTINATION include)

include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/align_utils-config.cmake.in
    ${PROJECT_BINARY_DIR}/align_utils-config.cmake
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME})
install(FILES ${PROJECT_BINARY_DIR}/align_utils-config.cmake
    DESTINATION lib/cmake/${PROJECT_NAME})
install(EXPORT align_utils-targets
    FILE align_utils-targets.cmake
    NAMESPACE compute_samples::
    DESTINATION lib/cmake/${PROJECT_NAME}
    COMPONENT align_utils)
