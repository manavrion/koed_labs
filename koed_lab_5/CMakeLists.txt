# CMakeList.txt : CMake project for koed_lab_5
cmake_minimum_required (VERSION 3.8)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add SOURCES variable
set(SOURCES main.cpp)

# Add include from koed_shared
include_directories("../koed_shared")

# Make executable file
add_executable(koed_lab_5 ${SOURCES})

# Link koed_shared
target_link_libraries(koed_lab_5 koed_shared)

# Copy resources
FILE(COPY "../resources/lab5_table.txt" DESTINATION "${CMAKE_BINARY_DIR}/koed_lab_5")
FILE(COPY "../resources/lab5_painter.exe" DESTINATION "${CMAKE_BINARY_DIR}/koed_lab_5")
