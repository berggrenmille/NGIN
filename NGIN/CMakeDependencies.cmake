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


# fmtlib
find_package(fmt 10 QUIET)
set(FMT_USE_NOEXCEPT ON CACHE BOOL "" FORCE)
fetch_dependency(fmt "https://github.com/fmtlib/fmt" "10.1.1")

# DYLIB
find_package(DYLIB QUIET)
fetch_dependency(DYLIB "https://github.com/martin-olivier/dylib" "v2.1.0")

# SDL2
find_package(SDL2 QUIET)
set(SDL_TEST_ENABLED_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_VULKAN ON CACHE BOOL "" FORCE)
fetch_dependency(SDL2 "https://github.com/libsdl-org/SDL" "release-2.28.1")

# GLM
find_package(glm QUIET)
fetch_dependency(glm "https://github.com/g-truc/glm" "0.9.9.8")

# JSON
find_package(nlohmann_json 3.11.2 QUIET)
set(JSON_BuildTests OFF CACHE INTERNAL "")
fetch_dependency(json "https://github.com/nlohmann/json" "v3.11.2")

# Google Test
find_package(googletest QUIET)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
fetch_dependency(googletest "https://github.com/google/googletest" "main")

# Vulkan
find_package(Vulkan QUIET)

# vulkan-hpp
#find_package(VulkanHeaders QUIET)
#if(UNIX)
#    set(VULKAN_HPP_NO_EXCEPTIONS ON CACHE BOOL "" FORCE)
#    fetch_dependency(vulkanHpp "https://github.com/KhronosGroup/Vulkan-Headers" "main")
#endif()