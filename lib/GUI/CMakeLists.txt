add_library(GUI STATIC)
target_sources(GUI PRIVATE
   src/Button.cpp 
)

target_link_libraries(GUI PUBLIC raylib)
target_include_directories(GUI PUBLIC 
   ${CMAKE_SOURCE_DIR}/lib/pch 
   ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_precompile_headers(GUI PRIVATE ${CMAKE_SOURCE_DIR}/lib/pch/Precompiled.h)

# silence raylib warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
   target_compile_options(GUI PRIVATE -w)
endif()