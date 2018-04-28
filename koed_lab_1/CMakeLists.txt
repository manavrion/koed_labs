# CMakeList.txt : CMake project for koed_lab_1
cmake_minimum_required (VERSION 3.8)

# Add SOURCES variable
set(SOURCES student_table.cpp table_reader.cpp main.cpp)

# Add include from koed_shared
include_directories("../koed_shared")

# Make executable
add_executable(koed_lab_1 ${SOURCES})

# Link koed_shared
target_link_libraries(koed_lab_1 koed_shared)

# Copy resource
FILE(COPY "../resources/table.txt" DESTINATION "${CMAKE_BINARY_DIR}/koed_lab_1")
FILE(COPY "../resources/student_table.txt" DESTINATION "${CMAKE_BINARY_DIR}/koed_lab_1")