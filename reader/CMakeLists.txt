cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)

set(SRCS reader.cpp)

add_executable(reader ${SRCS})
target_link_libraries(reader ipc-task-lib)
target_compile_options(reader PRIVATE -O2 -g)