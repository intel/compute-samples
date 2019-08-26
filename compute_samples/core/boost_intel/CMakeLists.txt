#
# Copyright(c) 2018 Intel Corporation
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
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
