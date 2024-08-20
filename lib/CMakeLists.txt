cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# Add any additional include directories here
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Create the library target
add_library(ipc-task-lib STATIC)
target_sources(ipc-task-lib PUBLIC ${HEADERS} PRIVATE ${SOURCES})

target_include_directories(ipc-task-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})