cmake_minimum_required(VERSION 3.10.2)

project(HTTPServer)

set(HTTPHANDLER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../HTTPHandler) 

include_directories(${HTTPHANDLER_INCLUDE_DIR})

set(SOURCE_LIB HTTPServer.cpp)

add_library(HTTPServer STATIC ${SOURCE_LIB})

target_link_libraries(HTTPServer HTTPHandler)
