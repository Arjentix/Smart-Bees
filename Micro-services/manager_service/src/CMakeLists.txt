cmake_minimum_required(VERSION 3.10.2)

project(manager_service_src)

set(HTTPHANDLER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/HTTPHandler) 
set(SERVER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/HTTPServer)

set(SOURCE_EXE main.cpp)

include_directories(${HTTPHANDLER_INCLUDE_DIR})

include_directories(${SERVER_INCLUDE_DIR})

add_executable(main ${SOURCE_EXE})

add_subdirectory(${SERVER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPServer)

add_subdirectory(${HTTPHANDLER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPHandler)

target_link_libraries(main HTTPHandler HTTPServer)

