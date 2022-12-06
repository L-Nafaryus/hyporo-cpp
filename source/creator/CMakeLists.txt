
project(
    hyporo
    VERSION 0.1.0
    LANGUAGES CXX
)

# Compiler options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Project options
include(GNUInstallDirs)


add_executable(${PROJECT_NAME}
    creator.cpp

        ../hpr/window_system/window_system.cpp
        ../hpr/window_system/monitor.cpp
        ../hpr/window_system/window.cpp
        ../hpr/window_system/glfw/window_system.cpp
        ../hpr/window_system/glfw/monitor.cpp
        ../hpr/window_system/glfw/window.cpp
)

include(${CMAKE_SOURCE_DIR}/cmake/imgui.cmake)
message(STATUS "project name: ${PROJECT_NAME}")
target_link_libraries(${PROJECT_NAME}
    hpr::hpr
    imgui
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ../
)
