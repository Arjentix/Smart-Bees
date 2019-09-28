project(simple_server)

cmake_minimum_required(VERSION 3.10.2)

include_directories("../../libs/HTTPHandler")

set(SOURCE_EXE main.cpp)
set(SERVER_LIB server.cpp)
set(DATABASE_LIB database.cpp)
set(HTTPHANDLER_LIB ../../libs/HTTPHandler/HTTPHandler.cpp)
set(COMPILER_FLAGS "-std=c++0x -pthread")
#set(COMPILER_FLAGS "-I/usr/include/mysql -DBIG_JOINS=1  -fno-strict-aliasing -g -L/usr/lib -lmysqlclient -lpthread -lz -lm -lrt -ldl")
#SET(GCC_COVERAGE_COMPILE_FLAGS "-DBIG_JOINS=1  -fno-strict-aliasing -g")
#SET(GCC_COVERAGE_LINK_FLAGS "-I/usr/include/mysql -L/usr/lib -lmysqlclient -lpthread -lz -lm -lrt -ldl")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_library(server STATIC ${SERVER_LIB})
add_library(handler STATIC ${HTTPHANDLER_LIB})
add_library(database STATIC ${DATABASE_LIB})

add_executable(main ${SOURCE_EXE})

add_definitions(${COMPILER_FLAGS})

target_link_libraries(main server handler database)
