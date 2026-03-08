add_library(Core STATIC)
target_sources(Core PRIVATE
   src/Event.cpp
   src/Layer.cpp
   src/LayerStack.cpp   
)

target_link_libraries(Core PUBLIC raylib)
target_include_directories(Core PUBLIC 
   ${CMAKE_SOURCE_DIR}/lib/pch 
   ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_precompile_headers(Core PRIVATE ${CMAKE_SOURCE_DIR}/lib/pch/Precompiled.h)