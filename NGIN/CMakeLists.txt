project(NGIN)

# Add all your source files here
add_library(NGIN SHARED

  "src/Core/App.cpp"
  "src/Core/Config.cpp"
  "src/Core/Layer.cpp"
  "src/Core/LayerStack.cpp"
  "src/Core/SDLWindow.cpp"
  "src/Core/Memory/Allocator.cpp"
  "src/Core/Memory/LinearAllocator.cpp"
  "src/Core/Memory/StackAllocator.cpp"
  "src/Core/Memory/FreeListAllocator.cpp"
  "src/Precompiled/PCH.cpp"
  "src/Core/Logging/ConsoleSink.cpp"
)
# Setup precompiled header
target_precompile_headers(NGIN PRIVATE include/Precompiled/PCH.h)
  
 if (MSVC)
    # Set /MD flag for Release configurations
    target_compile_options(NGIN PRIVATE $<$<CONFIG:Release>:/MD>)
    target_compile_options(NGIN PRIVATE $<$<CONFIG:Dist>:/MD>)
    # Set /MDd flag for Debug configurations
    target_compile_options(NGIN PRIVATE $<$<CONFIG:Debug>:/MDd>)
endif()

 

target_compile_definitions(NGIN PRIVATE NGIN_EXPORTS)


# Link the include folder
target_include_directories(NGIN PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(NGIN PRIVATE 
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)
# Link Thirdparty

target_link_libraries(NGIN PUBLIC loguru)
target_link_libraries(NGIN PUBLIC SDL2::SDL2-static SDL2::SDL2main)
target_link_libraries(NGIN PRIVATE dylib)
target_link_libraries(NGIN PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(NGIN PUBLIC glm)



