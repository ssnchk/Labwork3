cmake_minimum_required(VERSION 3.26)
project(Labwork-3)

set(CMAKE_CXX_STANDARD 14)

include_directories(bin)
include_directories(lib)

add_executable(Labwork_3
        bin/main.cpp
        lib/BMPWriter.cpp
        lib/SandParse.cpp
        lib/SandParse.h
        lib/SandPile.cpp
        lib/SandPile.h)
