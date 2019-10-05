cmake_minimum_required(VERSION 3.10.2)

project(subscribe_service_src)

set(SIMPLESQL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/SimpleSQL)
set(HTTPHANDLER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/HTTPHandler) 
set(SERVER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/HTTPServer)
set(DATABASE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/database)

set(SOURCE_EXE main.cpp)

include_directories(${SIMPLESQL_INCLUDE_DIR} ${HTTPHANDLER_INCLUDE_DIR})

include_directories(${SERVER_INCLUDE_DIR} ${DATABASE_INCLUDE_DIR})

add_executable(main ${SOURCE_EXE})

add_subdirectory(${SIMPLESQL_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/SimpleSQL)

add_subdirectory(${SERVER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPServer)

add_subdirectory(${DATABASE_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/database)

add_subdirectory(${HTTPHANDLER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPHandler)

target_link_libraries(main SimpleSQL HTTPHandler HTTPServer database)

