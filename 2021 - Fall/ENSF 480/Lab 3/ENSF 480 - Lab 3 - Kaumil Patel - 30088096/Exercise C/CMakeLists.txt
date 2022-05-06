cmake_minimum_required(VERSION 3.20)
project(Exercise_C)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Exercise_C
        customer.cpp
        customer.h
        lookupTable.h
        mainLab3ExC.cpp
        mystring2.cpp
        mystring2.h)
