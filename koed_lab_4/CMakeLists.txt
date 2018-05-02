# CMakeList.txt : CMake project for koed_lab_4
cmake_minimum_required (VERSION 3.8)

# Add SOURCES variable
set(SOURCES data_set.cpp main.cpp)

# Add include from koed_shared
include_directories("../koed_shared")

# Make executable
add_executable(koed_lab_4 ${SOURCES})

# Link koed_shared
target_link_libraries(koed_lab_4 koed_shared)

# Copy resource
FILE(COPY "../resources/lab4_test.txt" DESTINATION "${CMAKE_BINARY_DIR}/koed_lab_4")