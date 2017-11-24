cmake_minimum_required(VERSION 3.8)
project(framework)

set(SOURCES
    include/framework/framework.hpp
    src/framework.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(compute_samples::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    compute_samples::median_filter
    compute_samples::subgroups_imagecopy
    compute_samples::subgroups_visualization
    compute_samples::vme_search
    compute_samples::vme_hme
    compute_samples::vme_intra
    compute_samples::vme_wpp
    compute_samples::vme_interlaced
    compute_samples::vme_interop
    compute_samples::application
    Boost::log
    Boost::program_options
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER framework/${PROJECT_NAME})
