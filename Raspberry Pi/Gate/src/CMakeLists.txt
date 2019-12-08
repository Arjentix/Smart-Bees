cmake_minimum_required(VERSION 3.10.2)

project(gate_src)

set(SOURCE_EXE
		main.cpp
		AliceConnector.cpp
		LogPrinter.cpp
		MQTTPublisher.cpp
		PhotonConfigReader.cpp
		TokenHandler.cpp
)

# Adding ConfigReader
set(
	CONFIG_READER_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/ConfigReader
)
if(NOT TARGET ConfigReader)
	add_subdirectory(${CONFIG_READER_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/ConfigReader)
endif()

# Adding path to the FindMosquitto.cmake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Searching for Mosquitto lib
find_package(Mosquitto REQUIRED)

# Searching for Threads lib
find_package(Threads REQUIRED)

add_executable(gate ${SOURCE_EXE})

set_target_properties(gate PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS ON
)

target_include_directories(gate PUBLIC
	${CONFIG_READER_INCLUDE_DIR}
	${MOSQUITTO_INCLUDE_DIR}
)
target_link_libraries(gate
	ConfigReader
	${MOSQUITTO_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)

# Installing
install(TARGETS gate DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
