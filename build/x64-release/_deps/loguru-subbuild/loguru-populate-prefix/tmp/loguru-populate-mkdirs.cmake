# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Projects/NGIN/build/x64-release/_deps/loguru-src"
  "C:/Projects/NGIN/build/x64-release/_deps/loguru-build"
  "C:/Projects/NGIN/build/x64-release/_deps/loguru-subbuild/loguru-populate-prefix"
  "C:/Projects/NGIN/build/x64-release/_deps/loguru-subbuild/loguru-populate-prefix/tmp"
  "C:/Projects/NGIN/build/x64-release/_deps/loguru-subbuild/loguru-populate-prefix/src/loguru-populate-stamp"
  "C:/Projects/NGIN/build/x64-release/_deps/loguru-subbuild/loguru-populate-prefix/src"
  "C:/Projects/NGIN/build/x64-release/_deps/loguru-subbuild/loguru-populate-prefix/src/loguru-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Projects/NGIN/build/x64-release/_deps/loguru-subbuild/loguru-populate-prefix/src/loguru-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Projects/NGIN/build/x64-release/_deps/loguru-subbuild/loguru-populate-prefix/src/loguru-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
