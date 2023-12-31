﻿# CMakeList.txt : Top-level CMake project file
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

cmake_minimum_required(VERSION 3.8...3.27)
project("NGIN" VERSION 0.1)


# enable c and c++
enable_language(C CXX)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(NGIN_BUILD_TESTS "Build NGIN tests" OFF)
message(STATUS "Build NGIN tests: ${NGIN_BUILD_TESTS}")
option(NGIN_BUILD_DOCUMENTATION "Build NGIN documentation" OFF)
message(STATUS "Build documentation: ${NGIN_BUILD_DOCUMENTATION}")
option(NGIN_BUILD_ONLY_DOCUMENTATION "Build only documentation" OFF)
message(STATUS "Build only documentation: ${NGIN_BUILD_ONLY_DOCS}")
option(NIGN_TEST_ON_BUILD "Run tests on build" OFF)
message(STATUS "Run tests on build: ${NIGN_TEST_ON_BUILD}")

option(NGIN_USE_SDL "Use SDL" ON)
message(STATUS "Use SDL: ${NGIN_USE_SDL}")
option(NGIN_USE_GLFW "Use GLFW" OFF)
message(STATUS "Use GLFW: ${NGIN_USE_GLFW}")

if (NGIN_USE_SDL AND NGIN_USE_GLFW)
    message(FATAL_ERROR "Cannot use both SDL and GLFW")
endif ()

if (NGIN_BUILD_TESTS)
    set(BUILD_TESTING ON CACHE BOOL "" FORCE)
    include(CTest)
    enable_testing()
endif ()
# When building shared lib for Windows with MinGW,
# avoid the DLL having a "lib" prefix
set(CMAKE_SHARED_LIBRARY_PREFIX "")

#enable position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Setup definitions regarding third party libraries
if (NGIN_USE_SDL)
    add_compile_definitions(NGIN_USE_SDL)
endif ()
if (NGIN_USE_GLFW)
    add_compile_definitions(NGIN_USE_GLFW)
endif ()
#---------------------------------------------------------------------------------------------------------
# Specify different compiler flags and settings based on the build type
# Debug specific settings
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (MSVC)
        add_compile_options(/Zi /Ob0 /Od)
    else ()
        add_compile_options(-g)
    endif ()
endif ()
# RelWithDebInfo/Debug specific settings
if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    # Debug and RelWithDebInfo-specific settings
    add_compile_definitions(DEBUG)
    add_compile_definitions(NGIN_DEBUG)
    if (MSVC)
        add_compile_options(/MDd)
    else ()
        add_compile_options(-Wall -Wextra -pedantic)
    endif ()
endif ()
# RelWithDebInfo/Release specific settings
if (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    # Release-specific settings
    if (MSVC)
        add_compile_options(/O2)
    else ()
        add_compile_options(-O3)
    endif ()
endif ()
# Release specific settings
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (MSVC)
        add_compile_options(/MD)
    endif ()
    add_compile_definitions(NDEBUG)
endif ()


if (NOT NGIN_BUILD_ONLY_DOCUMENTATION)
    # Include sub-projects.
    add_subdirectory("NGIN")
    add_subdirectory("NGIN_EDITOR")

endif ()


if (NGIN_BUILD_DOCUMENTATION)
    add_subdirectory("DOCS")
endif ()
