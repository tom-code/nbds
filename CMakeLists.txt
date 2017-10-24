
cmake_minimum_required(VERSION 2.6)
project(nbds)

set (CMAKE_CXX_FLAGS "-g -std=c++11 -Wall -pedantic")

add_executable(nbds src/server.cc src/nbds.cc src/main.cc src/disk.cc)
target_link_libraries(nbds uv)
