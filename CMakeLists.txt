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
        add_compile_definitions(NDEBUG)
    endif()
endif()

#---------------------------------------------------------------------------------
# Third party libraries
include(FetchContent)
#fmtlib
FetchContent_Declare(
    fmt
    GIT_REPOSITORY "https://github.com/fmtlib/fmt"
    GIT_TAG "10.0.0"
)
FetchContent_MakeAvailable(fmt)

# DYLIB
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
SET(SDL_TEST OFF)
SET(SDL_VULKAN ON)
FetchContent_MakeAvailable(SDL)

# LOGURU
FetchContent_Declare(
    LOGURU
    GIT_REPOSITORY "https://github.com/emilk/loguru"
    GIT_TAG        "master"
)
SET(LOGURU_USE_FMTLIB TRUE)
FetchContent_MakeAvailable(LOGURU)


FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 0.9.9.8  # replace with the version you want
)

FetchContent_MakeAvailable(glm)



# JSON
FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.2
)
set(JSON_BuildTests OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(json)





# Include sub-projects.
add_subdirectory("NGIN")
add_subdirectory ("NGIN_EDITOR")

option(BUILD_DOCUMENTATION "Build documentation" OFF)
message(STATUS "Build documentation: ${BUILD_DOCUMENTATION}")
if (BUILD_DOCUMENTATION)
    add_subdirectory ("docs")
endif()