add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
   src/Layers/DailyLayer.cpp
   src/Layers/GameLayer.cpp
   src/Layers/HomeLayer.cpp
   src/Layers/MeLayer.cpp
   src/Layers/OptionsLayer.cpp
   src/Layers/PanelLayer.cpp
   src/App.cpp
   src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Engine)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Change debug logging based on preset
if(_DEBUGGING)
   target_compile_definitions(${PROJECT_NAME} PRIVATE _DEBUGGING=1)
else()
   target_compile_definitions(${PROJECT_NAME} PRIVATE _DEBUGGING=0)
endif()

# (MSVC has different warning flags)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
   # turn on all warnings for better debugging
   target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

   # disable missing struct initializers warnings
   target_compile_options(${PROJECT_NAME} PRIVATE -Wno-missing-field-initializers) 
endif()

# copy assets/ into build/ to remove dependencies and weird paths in code
add_custom_command(
   TARGET ${PROJECT_NAME} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
         ${CMAKE_SOURCE_DIR}/assets
         $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)