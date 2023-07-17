# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("NGIN")

# enable c and c++
enable_language(C CXX)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Third party libraries
include(FetchContent)
FetchContent_Declare(
  SDL2
  URL https://github.com/libsdl-org/SDL/releases/download/release-2.28.1/SDL2-2.28.1.tar.gz
)
FetchContent_MakeAvailable(SDL2)

# Include sub-projects.
add_subdirectory("NGIN_ENGINE")
add_subdirectory ("NGIN_EDITOR")


