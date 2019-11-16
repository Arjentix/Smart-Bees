cmake_minimum_required(VERSION 3.10.2)

project(manager_service_src)

# ------ Adding dependencies --------

# Adding HTTPHandler
set(
	HTTP_HANDLER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/HTTPHandler
)
if(NOT TARGET HTTPHandler)
	add_subdirectory(${HTTP_HANDLER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPHandler)
endif()

# Adding ConfigReader
set(
	CONFIG_READER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/ConfigReader
)
if(NOT TARGET ConfigReader)
	add_subdirectory(${CONFIG_READER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/ConfigReader)
endif()

# Adding Logger
set(
	LOGGER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/Logger
)
if(NOT TARGET Logger)
	add_subdirectory(${LOGGER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Logger)
endif()

# Adding json
set(
	JSON_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/json
)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
if(NOT TARGET nlohmann_json::nlohmann_json)
	add_subdirectory(
		${JSON_INCLUDE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}/json
	)
endif()

# Adding HTTPServer
set(
	HTTP_SERVER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/HTTPServer
)
if(NOT TARGET HTTPServer)
	add_subdirectory(${HTTP_SERVER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPServer)
endif()

# Adding HTTPClient
set(
	HTTP_CLIENT_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/HTTPClient
)
if(NOT TARGET HTTPClient)
	add_subdirectory(${HTTP_CLIENT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPClient)
endif()

set(SOURCE_EXE main.cpp)

# --------------------------

add_executable(ManagerService ${SOURCE_EXE})

set_target_properties(ManagerService PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS ON
)

target_include_directories(ManagerService PRIVATE
		${LOGGER_INCLUDE_DIR}
		${CONFIG_READER_INCLUDE_DIR}
		${HTTP_HANDLER_INCLUDE_DIR}
		${HTTP_SERVER_INCLUDE_DIR}
		${HTTP_CLIENT_INCLUDE_DIR}
		${JSON_INCLUDE_DIR}/include
)

target_link_libraries(ManagerService
		HTTPServer
		HTTPClient
		Logger
		ConfigReader
		HTTPHandler
		nlohmann_json::nlohmann_json
)

# Installing
install(TARGETS ManagerService DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
