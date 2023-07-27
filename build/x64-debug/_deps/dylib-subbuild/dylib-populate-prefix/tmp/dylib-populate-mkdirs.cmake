# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Projects/NGIN/build/x64-debug/_deps/dylib-src"
  "C:/Projects/NGIN/build/x64-debug/_deps/dylib-build"
  "C:/Projects/NGIN/build/x64-debug/_deps/dylib-subbuild/dylib-populate-prefix"
  "C:/Projects/NGIN/build/x64-debug/_deps/dylib-subbuild/dylib-populate-prefix/tmp"
  "C:/Projects/NGIN/build/x64-debug/_deps/dylib-subbuild/dylib-populate-prefix/src/dylib-populate-stamp"
  "C:/Projects/NGIN/build/x64-debug/_deps/dylib-subbuild/dylib-populate-prefix/src"
  "C:/Projects/NGIN/build/x64-debug/_deps/dylib-subbuild/dylib-populate-prefix/src/dylib-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Projects/NGIN/build/x64-debug/_deps/dylib-subbuild/dylib-populate-prefix/src/dylib-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Projects/NGIN/build/x64-debug/_deps/dylib-subbuild/dylib-populate-prefix/src/dylib-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
