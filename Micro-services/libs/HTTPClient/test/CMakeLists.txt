cmake_minimum_required(VERSION 3.10.2)

project(test)

set(CLIENT_SOURCE client.cpp)
set(SERVER_SOURCE server.cpp)

add_executable(client ${CLIENT_SOURCE})
add_executable(server ${SERVER_SOURCE})

# target_include_directories(client PUBLIC ..)
# target_include_directories(server PUBLIC ../../HTTPServer)

include_directories(.. ../../HTTPServer)
include_directories(.. ../../HTTPHandler)


add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_BINARY_DIR}/HTTPClient
)
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/../../HTTPServer
	${CMAKE_CURRENT_BINARY_DIR}/HTTPServer
)

target_link_libraries(client
	HTTPClient
)

target_link_libraries(server
	HTTPServer
)
