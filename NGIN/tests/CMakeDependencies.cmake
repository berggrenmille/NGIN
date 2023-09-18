# Dependencies Configuration
include(FetchContent)

macro(fetch_dependency _name _repo _tag)
    # Define the local path for the dependency
    set(local_path "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${_name}")

    # Append the local path to the CMAKE_PREFIX_PATH if it exists and the package hasn't been found yet
    if(EXISTS ${local_path} AND NOT ${_name}_FOUND)
        list(APPEND CMAKE_PREFIX_PATH ${local_path})
    endif()

    # Try to find the package
    find_package(${_name} QUIET)

    # If the package isn't found, use FetchContent to obtain it
    if(NOT ${_name}_FOUND)
        FetchContent_Declare(${_name}
            GIT_REPOSITORY ${_repo}
            GIT_TAG ${_tag}
        )
        FetchContent_MakeAvailable(${_name})
    endif()
endmacro()



# Google Test
find_package(googletest QUIET)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
fetch_dependency(googletest "https://github.com/google/googletest" "main")

