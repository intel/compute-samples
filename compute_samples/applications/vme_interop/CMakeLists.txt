cmake_minimum_required(VERSION 3.8)
project(vme_interop)

set(SOURCES
    include/vme_interop/vme_interop.hpp
    src/vme_interop.cpp
    vme_interop.cl
)

if(UNIX)
set(SOURCES
    ${SOURCES}
    src/vme_interop_linux.cpp
)
endif()

if(WIN32)
set(SOURCES
    ${SOURCES}
    src/vme_interop_windows.cpp
)
endif()

add_library(${PROJECT_NAME} ${SOURCES})
add_library(compute_samples::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    compute_samples::timer
    Boost::log
    Boost::program_options
    compute_samples::application
    compute_samples::yuv_utils
    compute_samples::align_utils
    compute_samples::boost_intel
)

if(UNIX)
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    X11 va va-drm va-x11
)
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

file(COPY "${MEDIA_DIRECTORY}/yuv/goal_1280x720.yuv" DESTINATION ${CMAKE_BINARY_DIR})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/vme_interop.cl" ${CMAKE_BINARY_DIR})

install(FILES "${MEDIA_DIRECTORY}/yuv/goal_1280x720.yuv" DESTINATION ${BIN_DIRECTORY})
install(FILES "${PROJECT_SOURCE_DIR}/vme_interop.cl" DESTINATION ${BIN_DIRECTORY})

source_group("Kernel Files" FILES vme_interop.cl)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER applications/${PROJECT_NAME})

set(TEST_SOURCES
    test/vme_interop_system_tests.cpp
    test/vme_interop_system_tests_common.hpp
)

if(UNIX)
set(TEST_SOURCES
    ${TEST_SOURCES}
    test/vme_interop_system_tests_linux.cpp)
endif()

if(WIN32)
set(TEST_SOURCES
    ${TEST_SOURCES}
    test/vme_interop_system_tests_windows.cpp)
endif()

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
add_executable(compute_samples::${PROJECT_NAME}_tests ALIAS ${PROJECT_NAME}_tests)

target_link_libraries(${PROJECT_NAME}_tests
    PUBLIC
    ${PROJECT_NAME}
    compute_samples::framework
    GTest::GTest
    GTest::Main
)

set_target_properties(${PROJECT_NAME}_tests PROPERTIES FOLDER applications/${PROJECT_NAME})

add_custom_command(TARGET ${PROJECT_NAME}_tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PROJECT_SOURCE_DIR}/vme_interop.cl" ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${MEDIA_DIRECTORY}/yuv/foreman_176x144.yuv" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${MEDIA_DIRECTORY}/yuv/interop_foreman_176x144.yuv" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS ${PROJECT_NAME}_tests DESTINATION ${BIN_DIRECTORY})
install(FILES "${MEDIA_DIRECTORY}/yuv/foreman_176x144.yuv" DESTINATION ${BIN_DIRECTORY})
install(FILES "${MEDIA_DIRECTORY}/yuv/interop_foreman_176x144.yuv" DESTINATION ${BIN_DIRECTORY})

add_test(${PROJECT_NAME}_tests, ${PROJECT_NAME}_tests)
