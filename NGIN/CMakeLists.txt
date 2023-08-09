project(NGIN)

# Add all your source files here
add_library(NGIN SHARED

  "include/Core/Layer.h"
  "include/Core/EventBus.h"
  "include/Core/LayerStack.h"
  "src/Core/Layer.cpp"
  "src/Core/LayerStack.cpp"
  "include/Core/Window.h"
  "src/Core/SDLWindow.cpp"
  "include/Core/Core.h"
  "include/Core/App.h"
  "include/NGIN.h"
  "include/Memory/Allocator.h"
  "include/Memory/LinearAllocator.h"
  "include/Core/Logger.h"
  "src/Core/App.cpp"
  
  "src/Memory/LinearAllocator.cpp"
  "src/Memory/Allocator.cpp"
  
  "include/Core/Config.h"
  "src/Core/Config.cpp" "include/Docs/Docs.h" "include/PCH.h" "src/PCH.cpp" "include/Memory/StackAllocator.h" "src/Memory/StackAllocator.cpp" "include/Memory/FreeListAllocator.h" "src/Memory/FreeListAllocator.cpp")

# Setup precompiled header
target_precompile_headers(NGIN PRIVATE include/PCH.h)
  
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
# Link Thirdparty

target_link_libraries(NGIN PUBLIC loguru)
target_link_libraries(NGIN PUBLIC SDL2::SDL2-static SDL2::SDL2main)
target_link_libraries(NGIN PRIVATE dylib)
target_link_libraries(NGIN PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(NGIN PUBLIC glm)



