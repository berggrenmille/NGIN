# CMakeList.txt : CMake project for NGIN, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable(NGIN_EDITOR
    src/main.cpp
    # Other source files...
)

# Link the include folder
target_include_directories(NGIN_EDITOR PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
    
# Link the executable to the NGIN_ENGINE library
target_link_libraries(NGIN_EDITOR PRIVATE NGIN_ENGINE)

# Copy dll from NGIN_ENGINE
add_custom_command(TARGET NGIN_EDITOR POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   $<TARGET_FILE:NGIN_ENGINE>
                   $<TARGET_FILE_DIR:NGIN_EDITOR>)