project(simple_server)

cmake_minimum_required(VERSION 3.10.2)

set(SOURCE_EXE main.cpp)

set(SOURCE_LIB server.cpp)

add_library(serv STATIC ${SOURCE_LIB})

add_executable(main ${SOURCE_EXE})

target_link_libraries(main serv)
