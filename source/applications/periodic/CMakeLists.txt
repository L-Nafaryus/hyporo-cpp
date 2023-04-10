cmake_minimum_required(VERSION 3.16)

project(periodic
    VERSION 0.1.0
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)
add_executable(periodic
    periodic.cpp
)

target_link_libraries(periodic
    hpr::csg
)

target_link_libraries(${PROJECT_NAME} -static gcc stdc++ winpthread -dynamic)
