cmake_minimum_required(VERSION 3.10.2)

set(SIMPLESQL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/SimpleSQL)
set(HTTPHANDLER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/HTTPHandler) 

project(subscribe_service_src)

set(SOURCE_EXE main.cpp server.cpp database.cpp)

include_directories(${SIMPLESQL_INCLUDE_DIR} ${HTTPHANDLER_INCLUDE_DIR})

add_executable(main ${SOURCE_EXE})

add_subdirectory(${SIMPLESQL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/SimpleSQL)

add_subdirectory(${HTTPHANDLER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPHandler)

target_link_libraries(main SimpleSQL HTTPHandler)

