cmake_minimum_required(VERSION 3.22)

project(ipc-task)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
add_subdirectory(reader)
add_subdirectory(writer)
add_subdirectory(tests)

