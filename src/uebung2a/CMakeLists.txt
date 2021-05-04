cmake_minimum_required(VERSION 3.19)
project(uebung2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(uebung2
        main.cpp
        unit.h
        unit.o)
