cmake_minimum_required(VERSION 3.10.2)

project(HTTPServer)

set(HTTPHANDLER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../HTTPHandler)
if(NOT TARGET HTTPHandler)
	add_subdirectory(${HTTPHANDLER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPHandler)
endif()

include_directories(${HTTPHANDLER_INCLUDE_DIR})

set(SOURCE_LIB HTTPServer.cpp)

add_library(HTTPServer STATIC ${SOURCE_LIB})

set_target_properties(HTTPServer PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS ON
)

target_link_libraries(HTTPServer HTTPHandler)
