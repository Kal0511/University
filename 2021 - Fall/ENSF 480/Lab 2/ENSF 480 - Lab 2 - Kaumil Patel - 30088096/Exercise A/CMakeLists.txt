cmake_minimum_required(VERSION 3.19)
project(Exercise_A)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Exercise_A
        dictionaryList.cpp
        exBmain.cpp
        mystring_B.cpp)
