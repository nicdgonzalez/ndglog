cmake_minimum_required(VERSION 3.2)

project(ndglog
  VERSION "0.1.0"
  DESCRIPTION "C implementation of a simple logging library."
  HOMEPAGE_URL "https://github.com/nicdgonzalez/ndglog"
)

set(NDGLOG_PUBLIC_H
  "ndglog/logging.h"
)

set(NDGLOG_SRCS
  "${NDGLOG_PUBLIC_H}"
  "ndglog/logging.c"
)

add_library(ndglog
  "${CMAKE_CURRENT_SOURCE_DIR}/ndglog/logging.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/ndglog/logging.h"
)

target_include_directories(ndglog
  INTERFACE
    "$<INSTALL_INTERFACE:include>"
    "$<BUILD_INTERFACE:{CMAKE_CURRENT_SOURCE_DIR}/include>"
)

install(
  TARGETS ndglog
  EXPORT ndglog-export
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install(
  EXPORT ndglog-export
  FILE "ndglog-targets.cmake"
  NAMESPACE "ndglog::"
  DESTINATION "lib/ndglog/cmake"
)

install(
  FILES
    "${NDGLOG_PUBLIC_H}"
  DESTINATION "include/ndglog"
)

install(
  FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ndglog-config.cmake"
  DESTINATION "lib/ndglog/cmake"
)
