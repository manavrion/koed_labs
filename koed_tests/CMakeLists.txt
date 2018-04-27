# CMakeList.txt : CMake project for koed_tests
cmake_minimum_required (VERSION 3.8)

# Add SOURCES variable
set(SOURCES ../koed_shared/matrix_unittest.cpp)

# Add include from koed_shared
include_directories("../koed_shared")
include_directories("../${gtest_SOURCE_DIR}/include")

# Make executable file
add_executable(koed_tests ${SOURCES})

# Link koed_shared
target_link_libraries(koed_tests koed_shared)
target_link_libraries(koed_tests gtest_main)

add_test(NAME koed_tests_global COMMAND koed_tests)
