cmake_minimum_required(VERSION 3.8)
project(boost_intel)

add_library(${PROJECT_NAME} INTERFACE)
add_library(compute_samples::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} 
    INTERFACE 
    ${SOURCES})

target_include_directories(${PROJECT_NAME}
    INTERFACE
    ${PROJECT_SOURCE_DIR}/include
)
