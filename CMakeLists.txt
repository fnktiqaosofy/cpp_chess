cmake_minimum_required(VERSION 3.14)
project(chess CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch FTXUI automatically
include(FetchContent)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG v6.1.9
)

FetchContent_MakeAvailable(ftxui)

file(GLOB_RECURSE SOURCES
    chess.cpp
    pieces/*.cpp
    utils/*.cpp
)

add_executable(chess ${SOURCES})

target_include_directories(chess PRIVATE pieces utils)

target_link_libraries(chess
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)
