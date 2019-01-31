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

function(add_application_library project_name)
    cmake_parse_arguments(F "" "" "SOURCE" ${ARGN})
    set(name "${project_name}_lib")
    add_library(${name} ${F_SOURCE})
    add_library(compute_samples::${name} ALIAS ${name})

    target_include_directories(${name}
        PUBLIC
        include
    )

    target_link_libraries(${name}
        PUBLIC
        compute_samples::application
    )

    set_target_properties(${name} PROPERTIES FOLDER applications/${project_name})
endfunction()

function(add_application project_name)
    cmake_parse_arguments(F "" "" "SOURCE" ${ARGN})
    set(name "${project_name}")
    add_executable(${name} ${F_SOURCE})
    add_executable(compute_samples::${name} ALIAS ${name})

    target_link_libraries(${name}
        PUBLIC
        compute_samples::${project_name}_lib
        compute_samples::logging
    )

    install(TARGETS ${name} DESTINATION ".")

    set_target_properties(${name} PROPERTIES FOLDER applications/${project_name})
endfunction()

function(add_application_test project_name)
    cmake_parse_arguments(F "" "" "SOURCE" ${ARGN})
    set(name "${project_name}_tests")
    add_executable(${name} ${F_SOURCE})
    add_executable(compute_samples::${name} ALIAS ${name})

    target_link_libraries(${name}
        PUBLIC
        compute_samples::${project_name}_lib
        GTest::GTest
    )

    install(TARGETS ${name} DESTINATION ".")

    set_target_properties(${name} PROPERTIES FOLDER applications/${project_name})

    add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endfunction()

function(add_kernels target)
    foreach(kernel ${ARGN})
        target_sources(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/${kernel}")
        source_group("Kernel Files" FILES "${CMAKE_CURRENT_SOURCE_DIR}/${kernel}")
    endforeach()
endfunction()

function(install_kernels target)
    foreach(kernel ${ARGN})
        add_custom_command(TARGET ${target} POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${kernel}" "${CMAKE_CURRENT_BINARY_DIR}")
        install(FILES "${kernel}" DESTINATION ".")
    endforeach()
endfunction()

function(install_resources target)
    cmake_parse_arguments(PARSED_ARGS "" "" "FILES;DIRECTORIES" ${ARGN})
    foreach(resource ${PARSED_ARGS_FILES})
        file(COPY "${resource}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
        install(FILES "${resource}" DESTINATION ".")
    endforeach()
    foreach(resource ${PARSED_ARGS_DIRECTORIES})
        file(COPY "${resource}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
        install(DIRECTORY "${resource}" DESTINATION ".")
    endforeach()
endfunction()

function(add_core_library name)
    cmake_parse_arguments(F "" "" "SOURCE" ${ARGN})
    add_library(${name} ${F_SOURCE})
    add_library(compute_samples::${name} ALIAS ${name})

    target_include_directories(${name}
        PUBLIC
        include
    )

    set_target_properties(${name} PROPERTIES FOLDER core/${name})
endfunction()

function(add_core_library_test project_name)
    cmake_parse_arguments(F "" "" "SOURCE" ${ARGN})
    set(name "${project_name}_tests")
    add_executable(${name} ${F_SOURCE})
    add_executable(compute_samples::${name} ALIAS ${name})

    target_link_libraries(${name}
        PUBLIC
        compute_samples::${project_name}
        GTest::GTest
    )

    install(TARGETS ${name} DESTINATION ".")

    set_target_properties(${name} PROPERTIES FOLDER core/${project_name})

    add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endfunction()
