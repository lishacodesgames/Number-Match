add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
   src/Layers/DailyLayer.cpp
   src/Layers/GameLayer.cpp
   src/Layers/HomeLayer.cpp
   src/Layers/MeLayer.cpp
   src/Game.cpp
   src/main.cpp
   src/MenuPanel.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE GUI Layers raylib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib/pch include)
target_precompile_headers( ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib/pch/Precompiled.h )

# turn on warnings for better debugging
# (but not for MSVC, which has different warning flags)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
   target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# copy assets/ into build/ to remove dependencies and weird paths in code
add_custom_command(
   TARGET ${PROJECT_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
         ${CMAKE_SOURCE_DIR}/assets
         $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)