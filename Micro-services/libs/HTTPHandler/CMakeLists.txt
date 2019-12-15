cmake_minimum_required(VERSION 3.10.2)

project(HTTPHandler)

set(SOURCE_LIB HTTPHandler.cpp)

add_library(HTTPHandler STATIC ${SOURCE_LIB})

set_target_properties(HTTPHandler PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS ON
)
