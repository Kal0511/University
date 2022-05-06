cmake_minimum_required(VERSION 3.20)
project(voting)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

add_executable(voting voting.cpp)
