cmake_minimum_required(VERSION 3.10)
project (summer-cc)
set (CMAKE_CXX_STANDARD 17)

add_executable (Game Source/main.cpp)

add_subdirectory (Dependencies)

target_link_libraries (Game LINK_PRIVATE raylib-ext)
