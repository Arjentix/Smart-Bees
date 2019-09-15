cmake_minimum_required(VERSION 3.10.2)

project(test)

set(SOURCE_EXE main.cpp)

include_directories(..)

add_executable(main ${SOURCE_EXE})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/HTTPHandler)

target_link_libraries(main HTTPHandler)
