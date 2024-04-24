cmake_minimum_required(VERSION 3.19)

project(Tree_contest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(Contest_files)
add_subdirectory(Tree)
add_subdirectory(Tests)
