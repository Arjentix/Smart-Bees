cmake_minimum_required(VERSION 3.10.2)

project(SendCommandRequestHandler_src)

# Adding RequestHandlerBase
set(
	REQUEST_HANDLER_BASE_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../RequestHandlerBase
)
if(NOT TARGET RequestHandlerBase)
	add_subdirectory(
		${REQUEST_HANDLER_BASE_INCLUDE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}/RequestHandlerBase
	)
endif()

# Adding json
set(
	JSON_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../../../../libs/json
)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
if(NOT TARGET nlohmann_json::nlohmann_json)
	add_subdirectory(
		${JSON_INCLUDE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}/json
	)
endif()


set(SOURCE SendCommandRequestHandler.cpp)

add_library(SendCommandRequestHandler ${SOURCE})

# set_target_properties(SendCommandRequestHandler PROPERTIES
# 		CXX_STANDARD 17
# 		CXX_STANDARD_REQUIRED ON
# 		CXX_EXTENSIONS ON
# )

target_include_directories(SendCommandRequestHandler PUBLIC
		${REQUEST_HANDLER_BASE_INCLUDE_DIR}
)
target_link_libraries(SendCommandRequestHandler
		RequestHandlerBase
		nlohmann_json::nlohmann_json
)
