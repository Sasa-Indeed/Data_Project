cmake_minimum_required(VERSION 3.23)
project(Scanner)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Scanner
        Scanner.cpp
        Scanner.h)
