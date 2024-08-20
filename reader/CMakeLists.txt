cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

set(SRCS reader.cpp)

add_executable(reader ${SRCS})
target_compile_options(reader PRIVATE -O2 -Werror -g)