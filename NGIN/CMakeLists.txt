project(NGIN)

# Add all your source files here
add_library(NGIN SHARED

  "include/Layer.h"
  "include/EventBus.h"
  "include/LayerStack.h"
  "src/Layer.cpp"
  "src/LayerStack.cpp"
  "include/Window.h"
  "src/SDLWindow.cpp"
  "include/Core.h"
  "include/App.h"
  "include/NGIN.h"
  "include/Memory/Allocator.h"
  "include/Memory/LinearAllocator.h"
  "include/Logger.h"
  "src/App.cpp"
  "src/EntryPoint.cpp"
  "src/Memory/LinearAllocator.cpp"
  "src/Memory/Allocator.cpp"
  "include/Detail/Docs.h"
  "include/Core/Config.h"
  "src/Config/Config.cpp")
  
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



