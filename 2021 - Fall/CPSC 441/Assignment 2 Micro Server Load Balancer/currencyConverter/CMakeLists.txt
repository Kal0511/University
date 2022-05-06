cmake_minimum_required(VERSION 3.20)
project(currencyConverter)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

add_executable(currencyConverter
        currencyConverter.cpp)
