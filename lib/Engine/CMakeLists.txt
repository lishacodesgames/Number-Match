add_library(Engine STATIC)
target_sources(Engine PRIVATE
   # Core
   Core/Event.cpp
   Core/Layer.cpp
   Core/LayerStack.cpp
   Core/Logging.cpp

   # GUI
   GUI/Button.cpp
   GUI/ScrollBar.cpp
   GUI/Toggle.cpp
   GUI/Animation.cpp
)

target_include_directories(Engine PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_SOURCE_DIR}/lib/external
)
target_link_libraries(Engine PUBLIC raylib)
target_precompile_headers(Engine PUBLIC pch/Precompiled.h)