cmake_minimum_required(VERSION 3.30)
project(CA3_CPP_FB)

set(CMAKE_CXX_STANDARD 20)

add_executable(CA3_CPP_FB main.cpp
        Crawler.cpp
        Crawler.h
        Board.cpp
        Board.h
        Bug.cpp
        Bug.h
        Hopper.cpp
        Hopper.h
        Jumper.cpp
        Jumper.h)

