cmake_minimum_required(VERSION 3.20)
project(Exercise_B)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Exercise_B
        iterator.cpp
        mystring2.cpp
        mystring2.h)
