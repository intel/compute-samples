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

cmake_minimum_required(VERSION 3.8)
project(median_filter)

set(LIB_SOURCES
    include/median_filter/median_filter.hpp
    src/median_filter.cpp
    median_filter.cl
)

add_library(${PROJECT_NAME}_lib ${LIB_SOURCES})
add_library(compute_samples::${PROJECT_NAME}_lib ALIAS ${PROJECT_NAME}_lib)

target_include_directories(${PROJECT_NAME}_lib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}_lib
    PUBLIC
    compute_samples::timer
    Boost::log
    compute_samples::application
    compute_samples::image
    compute_samples::version
    Boost::program_options
)

file(COPY "${MEDIA_DIRECTORY}/png/balloons_news.png" DESTINATION ${CMAKE_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME}_lib POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/median_filter.cl" ${CMAKE_BINARY_DIR})

install(FILES "${PROJECT_SOURCE_DIR}/median_filter.cl" DESTINATION ".")
install(FILES "${MEDIA_DIRECTORY}/png/balloons_news.png" DESTINATION ".")

source_group("Kernel Files" FILES median_filter.cl)
set_target_properties(${PROJECT_NAME}_lib PROPERTIES FOLDER applications/${PROJECT_NAME})

set(APP_SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})
add_executable(compute_samples::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_NAME}_lib
)

install(TARGETS ${PROJECT_NAME} DESTINATION ".")

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER applications/${PROJECT_NAME})

set(TEST_SOURCES
    test/median_filter_system_tests.cpp
)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
add_executable(compute_samples::${PROJECT_NAME}_tests ALIAS ${PROJECT_NAME}_tests)

target_link_libraries(${PROJECT_NAME}_tests
    PUBLIC
    ${PROJECT_NAME}_lib
    GTest::GTest
    GTest::Main
)

install(TARGETS ${PROJECT_NAME} DESTINATION ".")

set_target_properties(${PROJECT_NAME}_tests PROPERTIES FOLDER applications/${PROJECT_NAME})

file(COPY "${MEDIA_DIRECTORY}/png/test_input.png" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${MEDIA_DIRECTORY}/png/test_reference.png" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/median_filter.cl" ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS ${PROJECT_NAME}_tests DESTINATION ".")
install(FILES "${MEDIA_DIRECTORY}/png/test_input.png" DESTINATION ".")
install(FILES "${MEDIA_DIRECTORY}/png/test_reference.png" DESTINATION ".")

add_test(${PROJECT_NAME}_tests, ${PROJECT_NAME}_tests)
