cmake_minimum_required(VERSION 3.10.2)

set(SIMPLESQL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/SimpleSQL)

project(example_src)

set(SOURCE_EXE main.cpp)

include_directories(${SIMPLESQL_INCLUDE_DIR})

add_executable(main ${SOURCE_EXE})

add_subdirectory(${SIMPLESQL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/SimpleSQL)

target_link_libraries(main SimpleSQL)
