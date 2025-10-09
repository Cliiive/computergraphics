cmake_minimum_required(VERSION 3.16)
project(computergraphics LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Main executable
add_executable(computergraphics src/main.cpp)

# Enable testing for CTest
enable_testing()

# Add the tests folder as a subdirectory
add_subdirectory(tests)
