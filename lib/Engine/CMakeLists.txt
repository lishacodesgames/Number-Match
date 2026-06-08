add_library(Engine STATIC)
target_sources(Engine PRIVATE
   # Core
   src/Core/Event.cpp
   src/Core/Layer.cpp
   src/Core/LayerStack.cpp
   src/Core/Logging.cpp

   # GUI
   src/GUI/Button.cpp
   src/GUI/ScrollBar.cpp
)

target_include_directories(Engine PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_SOURCE_DIR}/lib/external
)
target_link_libraries(Engine PUBLIC raylib)
target_precompile_headers(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/pch/Precompiled.h)