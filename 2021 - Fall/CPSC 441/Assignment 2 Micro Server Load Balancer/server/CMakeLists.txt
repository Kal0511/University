cmake_minimum_required(VERSION 3.16)
project(server)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

add_executable(server server.cpp)
