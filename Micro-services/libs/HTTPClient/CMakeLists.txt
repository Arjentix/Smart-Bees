cmake_minimum_required(VERSION 3.10)

project(HTTPClient)

set(HTTPHANDLER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../HTTPHandler)

set(SOURCE_LIB HTTPClient.cpp)

add_library(HTTPClient ${SOURCE_LIB})
target_include_directories(HTTPClient PUBLIC ${HTTPHANDLER_INCLUDE_DIR})

if(NOT TARGET HTTPHandler)
    add_subdirectory(
        ${HTTPHANDLER_INCLUDE_DIR} 
        ${CMAKE_CURRENT_BINARY_DIR}/HTTPHandler
    )
endif()

target_link_libraries(HTTPClient HTTPHandler)



