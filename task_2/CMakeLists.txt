cmake_minimum_required(VERSION 3.16)
project(asteroids LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adjust if your MSYS2 path differs
set(SDL2_PATH "C:/msys64/mingw64")

include_directories("${SDL2_PATH}/include")
link_directories("${SDL2_PATH}/lib")
include_directories(${PROJECT_SOURCE_DIR}/include)

# Main executable
add_executable(raytracer src/raytracer/raytracer.cc
                         src/math/math.cc 
                         src/geometry/geometry.cc 
)

target_link_libraries(raytracer SDL2 SDL2_mixer)

# Enable testing for CTest
enable_testing()

# Add the tests folder as a subdirectory
add_subdirectory(tests)
