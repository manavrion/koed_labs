# CMakeList.txt : CMake project for koed_lab_5
cmake_minimum_required (VERSION 3.8)

# Add SOURCES variable
set(SOURCES main.cpp)

# Add include from koed_shared
include_directories("../koed_shared")

# Make executable file
add_executable(koed_lab_5 ${SOURCES})

# Link koed_shared
target_link_libraries(koed_lab_5 koed_shared)