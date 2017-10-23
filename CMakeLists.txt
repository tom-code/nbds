cmake_minimum_required(VERSION 2.6)
project(nbds)

set (CMAKE_CXX_FLAGS "-g -std=c++11 -Wall -pedantic")

add_executable(nbds server.cc nbds.cc)
target_link_libraries(nbds uv)
