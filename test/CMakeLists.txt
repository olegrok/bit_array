project(bit_array C)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_C_STANDARD 99)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    message("Clang ASAN is enabled for tests")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
else()
    message("ASAN is disabled for tests")
endif()

add_executable(main.test main.c)
target_link_libraries(main.test bit_array)

add_executable(arithmetic.test arithmetic.c)
target_link_libraries(arithmetic.test bit_array)

add_executable(comparison.test comparison.c)
target_link_libraries(comparison.test bit_array)

add_executable(interleave.test interleave.c)
target_link_libraries(interleave.test bit_array)

add_test(main ${CMAKE_CURRENT_BUILD_DIR}/main.test)
add_test(arithmetic ${CMAKE_CURRENT_BUILD_DIR}/arithmetic.test)
add_test(comparison ${CMAKE_CURRENT_BUILD_DIR}/comparison.test)
add_test(interleave ${CMAKE_CURRENT_BUILD_DIR}/interleave.test)

if(POLICY CMP0037)
    cmake_policy(SET CMP0037 OLD) # don't blame "test" target name
endif(POLICY CMP0037)

add_custom_target(test
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
    COMMAND ctest
    DEPENDS main.test arithmetic.test comparison.test interleave.test)
