cmake_minimum_required(VERSION 3.10.2)

project(database)

set(SIMPLESQL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../libs/SimpleSQL)

include_directories(${SIMPLESQL_INCLUDE_DIR})

#add_subdirectory(${SIMPLESQL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/SimpleSQL)

set(SOURCE_LIB database.cpp)

add_library(database STATIC ${SOURCE_LIB})

target_link_libraries(database SimpleSQL)
