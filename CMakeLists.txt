cmake_minimum_required(VERSION 3.14.1)

include(cmake/global_macros.cmake)

project(the_cplusplus_program)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

set(OUTPUT_BIN ${CMAKE_CURRENT_SOURCE_DIR}/output/bin)
set(OUTPUT_LIB ${CMAKE_CURRENT_SOURCE_DIR}/output/lib)

add_subdirectory(src)