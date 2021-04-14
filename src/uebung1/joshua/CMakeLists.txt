cmake_minimum_required(VERSION 3.19)
project(joshua)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(joshua
        meina1.cpp
        unit.h
        unit.o)
