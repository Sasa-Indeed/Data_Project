cmake_minimum_required(VERSION 3.23)
project(Parser)

set(CMAKE_CXX_STANDARD 14)

add_executable(Parser main.cpp cmake-build-debug/ParseTree.cpp cmake-build-debug/ParseTree.h)
