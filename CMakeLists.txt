# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("reorder_args")

add_executable (reorderargs "test/reorder_args.cpp" "include/reorder_args.hpp")
