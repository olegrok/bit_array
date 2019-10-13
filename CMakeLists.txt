project(bit_array C)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_C_STANDARD 99)

include(cmake/utils.cmake)

include_directories("${PROJECT_SOURCE_DIR}")
set(lib_headers
    bit_array.h
    bit_array_macros.h)

set(lib_sources
    bit_array.c)

add_library(${PROJECT_NAME} STATIC ${lib_sources})

add_compile_flags("C"
        "-Wall"
        "-Wextra"
        "-Wno-strict-aliasing"
        "-Werror")

enable_testing()
add_subdirectory(test)
