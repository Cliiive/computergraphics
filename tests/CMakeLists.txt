cmake_minimum_required (VERSION 3.10)
project (computergraphics)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

add_compile_options(-g -Wall -Wextra -Wpedantic -Wl,--stack,16777216)

# Add the math implementation to make sure the template code is available
add_executable(tests math_test.cc ${CMAKE_SOURCE_DIR}/src/math/math.cc)
target_link_libraries(tests gtest gtest_main)

# Make sure the test can find the implementation files
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_test(NAME tests COMMAND tests)