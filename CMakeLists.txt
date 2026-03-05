cmake_minimum_required(VERSION 3.20)
project(NumberMatch LANGUAGES C CXX) # EDIT your project name and languages 
# (DO NOT REMOVE C. RAYLIB IS IN C)

# Raylib uses C99, can update to any version after that
set(CMAKE_C_STANDARD 99) # raylib
set(CMAKE_CXX_STANDARD 20) # project
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# disable raylib examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# suppress unused result warning; enable missing field initializers warning
add_compile_options(-Wno-unused-result -Wmissing-field-initializers)

# go to cmakelists in subdirectories
add_subdirectory(lib/raylib)
add_subdirectory(lib/GUI)
add_subdirectory(lib/Layers)
add_subdirectory(App)