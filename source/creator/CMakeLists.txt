cmake_minimum_required (VERSION 3.16)

include(${CMAKE_SOURCE_DIR}/cmake/external/imgui.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/external/implot.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/external/glm.cmake)

project(
    hyporo-creator
    VERSION 0.1.0
    LANGUAGES CXX
)

# Compiler options
set(CMAKE_CXX_STANDARD 20)

# Project options
#include(GNUInstallDirs)

#include (InstallRequiredSystemLibraries)

if (MINGW)
    list(APPEND MINGW_SYSTEM_RUNTIME_LIBS
        "/mingw64/bin/libgcc_s_seh-1.dll"
        "/mingw64/bin/libwinpthread-1.dll"
        "/mingw64/bin/libstdc++-6.dll"
    )
endif()




#find_package(hpr REQUIRED)


add_executable(hyporo-creator
    test3.cpp
)

target_link_libraries(hyporo-creator
    hpr::gpu
    imgui::imgui
    implot::implot
    glm::glm
    hpr::csg
    hpr::mesh
)

target_link_libraries(hyporo-creator -static gcc stdc++ winpthread -dynamic)

