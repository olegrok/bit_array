project(bit_array C)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_C_STANDARD 99)

add_executable(main.test main.c)
target_link_libraries(main.test bit_array)
