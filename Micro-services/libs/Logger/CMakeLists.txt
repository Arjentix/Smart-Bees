cmake_minimum_required(VERSION 3.10.2)

project(Logger)

set(SOURCE_LIB Logger.cpp)

add_library(Logger STATIC ${SOURCE_LIB})

set_target_properties(Logger PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS ON
)
