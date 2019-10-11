cmake_minimum_required(VERSION 3.10.2)

set(SOURCE_EXE
		main.cpp
		ClientHandler.cpp
		EventManager.cpp
)

# Searching for Threads lib
find_package(Threads REQUIRED)

# Searching for json library
find_package(nlohmann_json 3.2.0 REQUIRED)

# Setting path to the local libraries
set(REQUESTHANDLERS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/RequestHandlers)
set(HTTPHANDLER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/HTTPHandler)
set(HTTPSERVER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/HTTPServer)

# Including local libraries
include_directories(
	${REQUESTHANDLERS_INCLUDE_DIR}
	${HTTPHANDLER_INCLUDE_DIR}
	${HTTPSERVER_INCLUDE_DIR}
)

add_executable(time_event_service ${SOURCE_EXE})

# Including CMakeLists.txt of local libraries
add_subdirectory(${REQUESTHANDLERS_INCLUDE_DIR})
add_subdirectory(${HTTPSERVER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/HTTPServer)

# Linking with all needed libraries
target_link_libraries(time_event_service
		${CMAKE_THREAD_LIBS_INIT}
		nlohmann_json::nlohmann_json
		HTTPHandler
		HTTPServer
)

# Installing
install(TARGETS time_event_service DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
