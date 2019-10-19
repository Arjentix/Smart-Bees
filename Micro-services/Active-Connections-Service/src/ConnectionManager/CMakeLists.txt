cmake_minimum_required(VERSION 3.10.2)

project(connection_manager)

# Adding HTTPServer
set(
	HTTPSERVER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/HTTPServer
)
if(NOT TARGET HTTPServer)
	add_subdirectory(${HTTPSERVER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPServer)
endif()

# Adding Logger
set(
	LOGGER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/Logger
)
if(NOT TARGET Logger)
	add_subdirectory(${LOGGER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Logger)
endif()

set(
	HTTPHANDLER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/HTTPHandler
)


set(SOURCE_CONNECTION_MANAGER
		ConnectionManager.cpp
)

add_library(ConnectionManager ${SOURCE_CONNECTION_MANAGER})

set_target_properties(ConnectionManager PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS ON
)

target_include_directories(ConnectionManager PUBLIC
		${HTTPSERVER_INCLUDE_DIR}
		${LOGGER_INCLUDE_DIR}
		${HTTPHANDLER_INCLUDE_DIR}
)

target_link_libraries(ConnectionManager
		HTTPServer
		Logger
)
