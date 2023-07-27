# CMakeList.txt : Top-level CMake project file
cmake_minimum_required (VERSION 3.8)
project ("NGIN" VERSION 0.1)


# enable c and c++
enable_language(C CXX)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#---------------------------------------------------------------------------------------------------------
# Specify different compiler flags and settings based on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug-specific settings
    add_compile_definitions(DEBUG)
    if(MSVC)
        add_compile_options(/Zi /Ob0 /Od)
    else()
        add_compile_options(-g)
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "Dist")
    # Release-specific settings
    if(MSVC)
        add_compile_options(/O2)
    else()
        add_compile_options(-O3)
    endif()
    if(CMAKE_BUILD_TYPE STREQUAL "Dist")
        add_compile_definitions(DNDEBUG)
    endif()
endif()

#---------------------------------------------------------------------------------
# Third party libraries
# DYLIB
include(FetchContent)
FetchContent_Declare(
    DYLIB
    GIT_REPOSITORY "https://github.com/martin-olivier/dylib"
    GIT_TAG        "v2.1.0"
)
FetchContent_MakeAvailable(DYLIB)

# SDL2
FetchContent_Declare(
    SDL
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL"
    GIT_TAG        "release-2.28.1"
)
#add_compile_definitions(SDL_MAIN_HANDLED)
FetchContent_MakeAvailable(SDL)

# LOGURU
FetchContent_Declare(
    LOGURU
    GIT_REPOSITORY "https://github.com/emilk/loguru"
    GIT_TAG        "master"
)


FetchContent_MakeAvailable(LOGURU)






# Include sub-projects.
add_subdirectory("NGIN")
add_subdirectory ("NGIN_EDITOR")

option(BUILD_DOCUMENTATION "Build documentation" OFF)
message(STATUS "Build documentation: ${BUILD_DOCUMENTATION}")
if (BUILD_DOCUMENTATION)
    add_subdirectory ("docs")
endif()