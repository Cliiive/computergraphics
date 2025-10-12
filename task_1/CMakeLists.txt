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
add_executable(main_game src/game/game.cc 
                         src/math/math.cc 
                         src/geometry/geometry.cc 
                         src/renderer/sdl2_renderer.cc 
                         src/sound/sound.cc 
                         src/game/main_game.cc 
                         src/physics/physics.cc 
                         src/game/sdl2_game_controller.cc 
                         src/timer/timer.cc
)

target_link_libraries(main_game SDL2 SDL2_mixer)

# Enable testing for CTest
enable_testing()

# Add the tests folder as a subdirectory
add_subdirectory(tests)
