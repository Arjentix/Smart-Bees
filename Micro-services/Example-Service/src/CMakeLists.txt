cmake_minimum_required(VERSION 3.10.2)

project(example_src)

set(SOURCE_EXE main.cpp)

# Adding path to the FindMySQL.cmake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Searching for MySQL lib
find_package(MySQL REQUIRED)

add_executable(main ${SOURCE_EXE})

include_directories(${MYSQL_INCLUDE_DIR})
target_link_libraries(main ${MYSQL_LIBRARIES})
