cmake_minimum_required(VERSION 3.2)
project(MyProject VERSION 0.1.0)
add_executable(MyApp "main.c")
find_package(ndglog CONFIG REQUIRED)
target_link_libraries(MyApp ndglog::ndglog)
