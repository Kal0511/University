cmake_minimum_required(VERSION 3.19)
project(Exercise_B)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Exercise_B
        dictionaryList.cpp
        dictionaryList.h
        exBmain.cpp
        mystring_B.cpp
        mystring_B.h)
