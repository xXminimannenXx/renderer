cmake_minimum_required(VERSION 3.25)
project(renderer C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Hämtar och bygger raylib automatiskt (första bygget tar en stund)
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_executable(renderer main.cpp Image.cpp Model.cpp Draw.cpp Display.cpp)
target_link_libraries(renderer PRIVATE raylib)