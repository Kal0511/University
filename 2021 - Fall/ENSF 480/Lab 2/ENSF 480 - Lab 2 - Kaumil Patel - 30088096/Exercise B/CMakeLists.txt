cmake_minimum_required(VERSION 3.19)
project(Exercise_B)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Exercise_B
        graphicsWorld.cpp main.cpp point.cpp point.h square.cpp square.h shape.cpp shape.h rectangle.cpp rectangle.h)
