cmake_minimum_required(VERSION 3.10.2)

project(Logger)

set(SOURCE_LIB Logger.cpp)

add_library(Logger STATIC ${SOURCE_LIB})
