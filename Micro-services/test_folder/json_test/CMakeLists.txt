cmake_minimum_required(VERSION 3.10.2)

project(json_test)

set(JSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/json)

set(SOURCE_EXE main.cpp)

include_directories(${JSON_INCLUDE_DIR})

add_executable(main ${SOURCE_EXE})

add_subdirectory(${JSON_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/json)

target_link_libraries(main json)

