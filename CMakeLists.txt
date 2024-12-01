cmake_minimum_required(VERSION 3.10)
project(Graph VERSION 1.0)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Graph ${SOURCES})

