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

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML-2.6.2/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics network REQUIRED)

include_directories(C:/SFML-2.6.2/include)
target_link_libraries(CA3_CPP_FB sfml-system sfml-window sfml-graphics)