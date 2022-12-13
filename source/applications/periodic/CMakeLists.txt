cmake_minimum_required(VERSION 3.16)

project(periodic
    VERSION "0.1.0"
    LANGUAGES CXX
)


add_executable(periodic
    periodic.cpp
)

target_link_libraries(periodic
    hpr::csg
)

