cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE Debug)
project(gdu VERSION 0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(gdu INTERFACE)
add_subdirectory(src)
target_include_directories(gdu INTERFACE include)
enable_testing()
include(GoogleTest)
add_subdirectory(test)
