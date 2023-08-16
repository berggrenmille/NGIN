project(NGIN)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Include Dependencies 
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeDependencies.cmake)
# Add all your source files here
add_library(NGIN SHARED

  "src/NGIN/App.cpp"
  "src/NGIN/Config.cpp"
  "src/NGIN/Layer.cpp"
  "src/NGIN/LayerStack.cpp"
  "src/NGIN/Memory/Allocator.cpp"
  "src/NGIN/Memory/LinearAllocator.cpp"
  "src/NGIN/Memory/StackAllocator.cpp"
  "src/NGIN/Memory/FreeListAllocator.cpp"
  "src/Precompiled/PCH.cpp"
  "src/NGIN/Logging/ConsoleSink.cpp"
  "src/NGIN/Logging/AsyncLogger.cpp"
  "src/NGIN/Logging/FileSink.cpp"
  "src/NGIN/Graphics/Vulkan/VulkanRenderer.cpp"
  "src/NGIN/Graphics/Vulkan/VulkanWindow.cpp"
  "src/NGIN/Graphics/Window.cpp" "src/NGIN/Logging.cpp" "src/NGIN/Logging/BaseLogger.cpp"  "src/Utils/JSON.cpp")


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



# Link to third-party libraries
target_link_libraries(NGIN PUBLIC 
    fmt::fmt 
    SDL2::SDL2 
    SDL2::SDL2main 
    nlohmann_json::nlohmann_json
    glm
    Vulkan::Vulkan
    Vulkan::Headers
)

target_link_libraries(NGIN PRIVATE dylib)



