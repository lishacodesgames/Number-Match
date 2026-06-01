cmake_minimum_required(VERSION 3.20)
project(NumberMatch LANGUAGES C CXX)

# Print the compiler path
message(STATUS "C Compiler:   ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")

# Print the compiler type (e.g., GNU, Clang, MSVC)
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

message(STATUS "--")

# go to cmakelists in subdirectories
add_subdirectory(lib/Engine)
add_subdirectory(lib/raylib)
add_subdirectory(App)