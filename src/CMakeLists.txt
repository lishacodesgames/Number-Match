file(GLOB_RECURSE NM_SOURCES *.cpp *.h)
add_executable(${PROJECT_NAME} ${NM_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Engine)

set(BIN ${CMAKE_SOURCE_DIR}/bin/${OUTFILE})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Turn on all warnings, and disable harmless, annoying ones
if (MSVC)
   target_compile_options(${PROJECT_NAME} PRIVATE /W4)
   target_compile_options(${PROJECT_NAME} PRIVATE /wd4458 /wd4244 /wd4267)
else()
   target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
   target_compile_options(${PROJECT_NAME} PRIVATE -Wno-missing-field-initializers -Wno-unused-result -Wno-shadow)
endif()

# copy assets/ into bin/ to remove dependencies and weird paths in code
add_custom_target(copy_assets ALL
   COMMAND ${CMAKE_COMMAND} -E copy_directory
   ${CMAKE_SOURCE_DIR}/assets ${BIN}/assets
   COMMENT "Copying assets to output directory"
)
add_dependencies(${PROJECT_NAME} copy_assets)