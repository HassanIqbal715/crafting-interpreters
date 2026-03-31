cmake_minimum_required(VERSION 3.28.3)

project("LOX")

set(C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/bin/")

include_directories("include")
add_subdirectory(src)