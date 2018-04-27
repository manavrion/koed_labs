# CMakeList.txt : CMake project for koed_shared
cmake_minimum_required (VERSION 3.8)

# Add SOURCES variable
set(SOURCE matrix_alg.h matrix.h matrix.cpp)

# Make library file
add_library(koed_shared STATIC ${SOURCE})
