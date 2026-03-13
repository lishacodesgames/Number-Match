add_subdirectory(Engine)

# disable raylib examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# add raylib and suppress all warnings 
add_subdirectory(raylib raylib_build SYSTEM)