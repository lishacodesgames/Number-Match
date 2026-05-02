cmake_minimum_required(VERSION 3.20)
project(NumberMatch LANGUAGES C CXX)

# go to cmakelists in subdirectories
add_subdirectory(lib/Engine)
add_subdirectory(lib/raylib)
add_subdirectory(App)