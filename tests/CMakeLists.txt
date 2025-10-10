cmake_minimum_required (VERSION 3.10)
project (computergraphics)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

add_compile_options(-g -Wall -Wextra -Wpedantic -Wl,--stack,16777216)

# Create separate test executables
# Math tests
add_executable(math_tests math_test.cc ${CMAKE_SOURCE_DIR}/src/math/math.cc)
target_link_libraries(math_tests gtest gtest_main)
target_include_directories(math_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME math_tests COMMAND math_tests)

# Geometry tests
add_executable(geometry_tests geometry_test.cc ${CMAKE_SOURCE_DIR}/src/geometry/geometry.cc ${CMAKE_SOURCE_DIR}/src/math/math.cc)
target_link_libraries(geometry_tests gtest gtest_main)
target_include_directories(geometry_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME geometry_tests COMMAND geometry_tests)

# Physics tests
add_executable(physics_tests physics_test.cc 
                              ${CMAKE_SOURCE_DIR}/src/physics/physics.cc 
                              ${CMAKE_SOURCE_DIR}/src/math/math.cc
                              ${CMAKE_SOURCE_DIR}/src/geometry/geometry.cc
                              ${CMAKE_SOURCE_DIR}/src/timer/timer.cc
                              )
target_link_libraries(physics_tests gtest gtest_main)
target_include_directories(physics_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME physics_tests COMMAND physics_tests)

# Physics tests
add_executable(game_test game_test.cc
                              ${CMAKE_SOURCE_DIR}/src/game/game.cc
                              ${CMAKE_SOURCE_DIR}/src/physics/physics.cc 
                              ${CMAKE_SOURCE_DIR}/src/math/math.cc
                              ${CMAKE_SOURCE_DIR}/src/geometry/geometry.cc
                              ${CMAKE_SOURCE_DIR}/src/timer/timer.cc
                              )
target_link_libraries(game_test gtest gtest_main)
target_include_directories(game_test PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME game_test COMMAND game_test)

# All tests combined
add_executable(all_tests math_test.cc geometry_test.cc ${CMAKE_SOURCE_DIR}/src/math/math.cc ${CMAKE_SOURCE_DIR}/src/geometry/geometry.cc)
target_link_libraries(all_tests gtest gtest_main)
target_include_directories(all_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME all_tests COMMAND all_tests)