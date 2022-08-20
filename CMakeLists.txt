cmake_minimum_required(VERSION 3.2)

find_program(CMAKE_C_COMPILER
  NAMES %ENV{CC} "clang"
  PATHS ENV "PATH"
  NO_DEFAULT_PATH
)

find_program(CMAKE_CXX_COMPILER
  NAMES %ENV{CXX} "clang++"
  PATHS ENV "PATH"
  NO_DEFAULT_PATH
)

find_program(CMAKE_MAKE_PROGRAM
  NAMES %ENV{MAKE} "make"
  PATHS ENV "PATH"
  NO_DEFAULT_PATH
)

project(ndglog
  VERSION "0.1.0"
  DESCRIPTION "C implementation of a simple logging library."
  HOMEPAGE_URL "https://github.com/nicdgonzalez/ndglog"
)

add_library(ndglog
  "${CMAKE_CURRENT_SOURCE_DIR}/ndglog/logging.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/ndglog/logging.h"
)

install(
  FILES "ndglog/logging.h"
  DESTINATION "include/ndglog"
)

install(
  TARGETS
    ndglog
  EXPORT ndglog-export
  DESTINATION "lib"
)

install(EXPORT ndglog-export
  FILE "ndglog-config.cmake"
  NAMESPACE "ndglog::"
  DESTINATION "lib/ndglog/cmake"
)
