cmake_minimum_required(VERSION 3.20)
project(NumberMatch LANGUAGES C CXX)

# Raylib uses C99, can update to any version after that
set(CMAKE_C_STANDARD 99) # raylib
set(CMAKE_CXX_STANDARD 20) # project
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# go to cmakelists in subdirectories
add_subdirectory(lib)
add_subdirectory(App)