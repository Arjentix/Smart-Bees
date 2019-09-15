cmake_minimum_required(VERSION 3.10.2)

project(HTTPHandler)

set(SOURCE_LIB HTTPHandler.cpp)

add_library(HTTPHandler STATIC ${SOURCE_LIB})
