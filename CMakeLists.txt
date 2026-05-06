cmake_minimum_required(VERSION 3.10)
project(LinwisEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

file(GLOB ENGINE_SOURCES
    "engine/src/math/*.cpp"
    "engine/src/core/*.cpp"
    "engine/src/scene/*.cpp"
    "engine/src/graphics/*.cpp"
    "engine/src/helpers/*.cpp"
    "engine/src/platform/win32/Win32Window.cpp"
)
file(GLOB GAME_SOURCES "game/game.cpp")

configure_file(
    "${PROJECT_SOURCE_DIR}/engine/include/lw/core/PathsConfig.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/lw/core/PathsConfig.h"
    @ONLY
)

add_executable(Game ${ENGINE_SOURCES} ${GAME_SOURCES})

target_include_directories(Game PRIVATE
    "${PROJECT_SOURCE_DIR}/engine/include"
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)

target_link_libraries(Game PRIVATE gdi32)
