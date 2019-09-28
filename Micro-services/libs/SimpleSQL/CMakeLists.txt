cmake_minimum_required(VERSION 3.10.2)

project(SimpleSQL)

set(SOURCE_LIB SimpleSQL.cpp)

# Adding path to the FindMySQL.cmake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Searching for MySQL lib
find_package(MySQL REQUIRED)

add_library(SimpleSQL STATIC ${SOURCE_LIB})

target_include_directories(SimpleSQL PUBLIC ${MYSQL_INCLUDE_DIR})
target_link_libraries(SimpleSQL ${MYSQL_LIBRARIES})
