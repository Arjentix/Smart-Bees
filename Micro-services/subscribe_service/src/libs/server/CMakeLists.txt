cmake_minimum_required(VERSION 3.10.2)

project(server)

set(HTTPHANDLER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../libs/HTTPHandler) 

include_directories(${HTTPHANDLER_INCLUDE_DIR})

set(SOURCE_LIB server.cpp)

#add_subdirectory(${HTTPHANDLER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPHandler)

add_library(server STATIC ${SOURCE_LIB})

target_link_libraries(server HTTPHandler)
