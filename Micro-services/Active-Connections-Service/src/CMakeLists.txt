cmake_minimum_required(VERSION 3.10.2)

project(animal_store_src)

# ------ Adding dependencies --------

# Adding RequestHandlerBase
set(
	REQUEST_HANDLER_BASE_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/RequestHandlers/RequestHandlerBase/
)
if(NOT TARGET RequestHandler)
	add_subdirectory(${REQUEST_HANDLER_BASE_INCLUDE_DIR})
endif()

# Adding HTTPHandler
set(
	HTTP_HANDLER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/HTTPHandler
)
if(NOT TARGET HTTPHandler)
	add_subdirectory(${HTTP_HANDLER_INCLUDE_DIR})
endif()

# Adding Logger
set(
	LOGGER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/Logger
)
if(NOT TARGET Logger)
	add_subdirectory(${LOGGER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Logger)
endif()

# Adding HTTPServer
set(
	HTTP_SERVER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/HTTPServer
)
if(NOT TARGET HTTPServer)
	add_subdirectory(${HTTP_SERVER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPServer)
endif()

# Adding ConnectionManager
set(
	CONNECTION_MANAGER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/ConnectionManager
)
if(NOT TARGET ConnectionManager)
	add_subdirectory(${CONNECTION_MANAGER_INCLUDE_DIR})
endif()

# Adding SendCommandRequestHandler
set(
	SEND_COMMAND_REQUEST_HANDLER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/RequestHandlers/SendCommandRequestHandler
)
if(NOT TARGET SendCommandRequestHandler)
	add_subdirectory(${SEND_COMMAND_REQUEST_HANDLER_INCLUDE_DIR})
endif()

# Adding nlohmann/detail/exceptions
set(
	JSON_EXCEPTIONS_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/json/include
)

# Searching for Threads lib
find_package(Threads REQUIRED)

# -------------------------


set(SOURCE_EXE
		main.cpp
		ClientHandler.cpp
)

add_executable(ActiveConnectionsService ${SOURCE_EXE})

set_target_properties(ActiveConnectionsService PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS ON
)

target_include_directories(ActiveConnectionsService PRIVATE
		${REQUEST_HANDLER_BASE_INCLUDE_DIR}
		${LOGGER_INCLUDE_DIR}
		${HTTP_HANDLER_INCLUDE_DIR}
		${HTTP_SERVER_INCLUDE_DIR}
		${CONNECTION_MANAGER_INCLUDE_DIR}
		${SEND_COMMAND_REQUEST_HANDLER_INCLUDE_DIR}
		${JSON_EXCEPTIONS_INCLUDE_DIR}
)

target_link_libraries(ActiveConnectionsService
		${CMAKE_THREAD_LIBS_INIT}
		RequestHandlerBase
		HTTPServer
		ConnectionManager
		SendCommandRequestHandler
		Logger
)

# Installing
install(TARGETS ActiveConnectionsService DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
