cmake_minimum_required(VERSION 3.10.2)

project(request_handlers)

# Adding HTTPHandler
set(
	HTTPHANDLER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../HTTPHandler
)
if(NOT TARGET HTTPHandler)
	add_subdirectory(${HTTPHANDLER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPHandler)
endif()

# Adding ConnectionManager
set(
	CONNECTION_MANAGER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../ConnectionManager
)
if(NOT TARGET ConnectionManager)
	add_subdirectory(${CONNECTION_MANAGEG_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/ConnectionManager)
endif()


set(SOURCE_REQUEST_HANDLER_BASE
		RequestHandlerBase.cpp
)

add_library(RequestHandlerBase ${SOURCE_REQUEST_HANDLER_BASE})

set_target_properties(RequestHandlerBase PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS ON
)

target_include_directories(RequestHandlerBase PUBLIC
	${HTTPHANDLER_INCLUDE_DIR}
	${CONNECTION_MANAGER_INCLUDE_DIR}
)

target_link_libraries(RequestHandlerBase
		HTTPHandler
		ConnectionManager
)
