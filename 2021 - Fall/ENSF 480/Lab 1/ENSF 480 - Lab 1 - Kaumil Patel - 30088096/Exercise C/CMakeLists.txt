cmake_minimum_required(VERSION 3.19)
project(Exercise_C)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Exercise_C
        Address.h
        company.cpp
        Customer.h
        Date.h
        Employee.h)
