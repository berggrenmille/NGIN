project(NGIN)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Include Dependencies 
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeDependencies.cmake)

# Add all your source files here
add_library(NGIN SHARED

  "src/NGIN/App.cpp"
  "src/NGIN/Config.cpp"
  "src/NGIN/Memory/LinearAllocator.cpp"
  "src/NGIN/Memory/StackAllocator.cpp"
  "src/NGIN/Memory/FreeListAllocator.cpp"
  "src/NGIN/Memory/Mallocator.cpp"
  "src/Precompiled/PCH.cpp"
  "src/NGIN/Logging/ConsoleSink.cpp"
  "src/NGIN/Logging/AsyncLogger.cpp"
  "src/NGIN/Logging/FileSink.cpp"
  "src/NGIN/Graphics/Vulkan/VulkanRenderer.cpp"
  "src/NGIN/Graphics/Window.cpp" 
  "src/NGIN/Logging.cpp" 
  "src/NGIN/Logging/BaseLogger.cpp"  
  "src/Utils/JSON.cpp"
  "src/NGIN/Meta/UUID.cpp"
  "src/NGIN/Meta/Reflection/Registry.cpp"
  "src/NGIN/Graphics/Context.cpp"
  "src/NGIN/Core/Engine.cpp"
)


# Setup precompiled header
target_precompile_headers(NGIN PRIVATE include/Precompiled/PCH.h)
  
 

target_compile_definitions(NGIN PRIVATE NGIN_EXPORTS)


# Link the include folder
target_include_directories(NGIN PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(NGIN PRIVATE 
    ${PROJECT_SOURCE_DIR}/include/Precompiled
)
# Link Thirdparty
message (STATUS "RAPIDJSON_SOURCE_DIR: ${rapidjson_SOURCE_DIR}")
target_include_directories(NGIN PUBLIC
    ${rapidjson_SOURCE_DIR}/include
)


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


if(WIN32) # Check if we are on Windows


    # Get the path to the SDL2 binary
    set(SDL2_DLL_PATH $<TARGET_FILE:SDL2::SDL2>)

    message(STATUS "SDL2.dll path: ${SDL2_DLL_PATH}")

    # Copy SDL2.dll after build if it's different
    add_custom_command(TARGET NGIN POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_DLL_PATH}"
        $<TARGET_FILE_DIR:NGIN>
    )

endif()

if(NGIN_BUILD_TESTS)

    add_subdirectory("tests")
endif()





