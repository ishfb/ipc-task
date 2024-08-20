cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)

set(SRCS writer.cpp)

add_executable(writer ${SRCS})
target_link_libraries(writer ipc-task-lib)
target_compile_options(writer PRIVATE -O2 -g)