add_library(Layers STATIC)
target_sources(Layers PRIVATE
   src/Event.cpp
   src/Layer.cpp
   src/LayerStack.cpp   
)

target_link_libraries(Layers PUBLIC raylib)
target_include_directories(Layers PUBLIC 
   ${CMAKE_SOURCE_DIR}/lib/pch 
   ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_precompile_headers(Layers PRIVATE ${CMAKE_SOURCE_DIR}/lib/pch/Precompiled.h)