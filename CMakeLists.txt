cmake_minimum_required (VERSION 3.10)
project (math)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-g -Wall -Wextra -Wpedantic -Wl,--stack,16777216)

include_directories(include)

add_executable(math_test test/math_test.cc src/math/math.cc)
target_link_libraries(math_test gtest gtest_main)

