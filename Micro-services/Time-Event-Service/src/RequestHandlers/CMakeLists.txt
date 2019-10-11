cmake_minimum_required(VERSION 3.10.2)

project(request_handlers)

set(SOURCE_REQUEST_HANDLERS
		RequestHanderBase.cpp
		GetRequestHandler.cpp
		PostRequestHandler.cpp
		DeleteRequestHandler.cpp
)

set(HTTPHANDLER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/HTTPHandler)

include_directories(
	${HTTPHANDLER_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_library(request_handlers ${SOURCE_REQUEST_HANDLERS})

add_subdirectory(${HTTPHANDLER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPHandler)

target_link_libraries(request_handlers HTTPHandler)
