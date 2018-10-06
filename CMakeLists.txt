cmake_minimum_required(VERSION 3.0)

project(crowd VERSION 1.0.0)

find_package (Qt5 COMPONENTS Core Gui Widgets REQUIRED)

enable_language (CXX)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -pedantic")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config/config.json ${CMAKE_CURRENT_BINARY_DIR}/src/config.json COPYONLY)

add_subdirectory (src)
