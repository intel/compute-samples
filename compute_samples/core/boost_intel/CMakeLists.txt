#
# Copyright (C) 2019 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_library(boost_intel INTERFACE)
add_library(compute_samples::boost_intel ALIAS boost_intel)
target_include_directories(boost_intel
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(boost_intel
    INTERFACE
    compute_samples::ocl_entrypoints
)

install(TARGETS boost_intel
    EXPORT boost_intel-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/boost DESTINATION include)

include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/boost_intel-config.cmake.in
    ${PROJECT_BINARY_DIR}/boost_intel-config.cmake
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME})
install(FILES ${PROJECT_BINARY_DIR}/boost_intel-config.cmake
    DESTINATION lib/cmake/${PROJECT_NAME})
install(EXPORT boost_intel-targets
    FILE boost_intel-targets.cmake
    NAMESPACE compute_samples::
    DESTINATION lib/cmake/${PROJECT_NAME}
    COMPONENT boost_intel)
