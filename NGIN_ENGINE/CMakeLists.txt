project(NGIN_ENGINE)

# Add all your source files here
add_library(NGIN_ENGINE SHARED
    src/engine.cpp
 "include/Layer.h" "include/EventBus.h" "include/LayerStack.h" "src/Layer.cpp" "src/LayerStack.cpp")

target_compile_definitions(NGIN_ENGINE PRIVATE NGIN_ENGINE_EXPORTS)


# Link the include folder
target_include_directories(NGIN_ENGINE PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
# Link SDL2
target_link_libraries(NGIN_ENGINE SDL2::SDL2)