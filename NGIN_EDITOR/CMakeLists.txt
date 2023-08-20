# CMakeList.txt : CMake project for NGIN, include source and define
# project specific logic here.
#
project(NGIN_EDITOR)
# Add source to this project's executable.
add_executable(NGIN_EDITOR
    src/EditorApp.cpp
    # Other source files...
 )

# Link the include folder
target_include_directories(NGIN_EDITOR PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
    
# Link the executable to the NGIN library 
target_link_libraries(NGIN_EDITOR PUBLIC NGIN)

# Copy all dlls from NGIN
add_custom_command(TARGET NGIN_EDITOR POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    -DDST_DIR="$<TARGET_FILE_DIR:NGIN_EDITOR>"
    -DSRC_DIR="$<TARGET_FILE_DIR:NGIN>"
    -P ${CMAKE_SOURCE_DIR}/CMakeCopyDll.cmake
)
