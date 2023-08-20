# CMakeList.txt : Top-level CMake project file
cmake_minimum_required (VERSION 3.8)
project ("NGIN" VERSION 0.1)


# enable c and c++
enable_language(C CXX)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(NGIN_BUILD_TESTS "Build NGIN tests" OFF)
message(STATUS "Build NGIN tests: ${NGIN_BUILD_TESTS}")
option(NGIN_BUILD_DOCUMENTATION "Build NGIN documentation" OFF)
message(STATUS "Build documentation: ${NGIN_BUILD_DOCUMENTATION}")

# When building shared lib for Windows with MinGW,
# avoid the DLL having a "lib" prefix
if(WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")

endif()

#---------------------------------------------------------------------------------------------------------
# Specify different compiler flags and settings based on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug-specific settings
    add_compile_definitions(DEBUG)
    add_compile_definitions(NGIN_DEBUG)
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







# Include sub-projects.
add_subdirectory("NGIN")
add_subdirectory ("NGIN_EDITOR")


if(NGIN_BUILD_TESTS)
    add_subdirectory("TESTS")
endif()
if (NGIN_BUILD_DOCUMENTATION)
    add_subdirectory ("DOCS")
endif()
