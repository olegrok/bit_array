project(bit_array C)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_C_STANDARD 99)

include(cmake/utils.cmake)
include(CheckCSourceCompiles)

include_directories("${PROJECT_SOURCE_DIR}")
set(lib_headers
    bit_array.h
    bit_array_interleave.h
    bit_array_macros.h)

set(lib_sources
    bit_array_interleave.c
    bit_array.c)

add_library(${PROJECT_NAME} STATIC ${lib_sources})

add_compile_flags("C"
        "-Wall"
        "-Wextra"
        "-Wno-strict-aliasing"
        "-Werror"
        "-O3")
#
# Check for an omp support
#
set(CMAKE_REQUIRED_FLAGS "-fopenmp-simd -Werror")
check_c_source_compiles("int main(void) {
#pragma omp simd
    {
    }
    return 0;
}" HAVE_OPENMP)

set(CMAKE_REQUIRED_FLAGS "")

if (NOT HAVE_OPENMP)
    add_compile_flags("C" "-Wno-unknown-pragmas")
else(HAVE_OPENMP)
    add_compile_flags("C" "-fopenmp-simd")
endif()

enable_testing()
add_subdirectory(test)
