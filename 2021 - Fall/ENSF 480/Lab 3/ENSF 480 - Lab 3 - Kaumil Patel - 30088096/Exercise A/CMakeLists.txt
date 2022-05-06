cmake_minimum_required(VERSION 3.19)
project(Exercise_A)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Exercise_A
        graphicsWorld.cpp main.cpp point.cpp point.h square.cpp square.h circle.cpp circle.h shape.cpp shape.h rectangle.cpp rectangle.h curveCut.cpp curveCut.h)
