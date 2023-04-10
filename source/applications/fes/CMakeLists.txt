cmake_minimum_required (VERSION 3.16)

include(${CMAKE_SOURCE_DIR}/cmake/external/imgui.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/external/implot.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/external/openxlsx.cmake)

project(
       fes
        VERSION 0.1.0
        LANGUAGES CXX
)

# Compiler options
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
        fes.cpp
        )

target_link_libraries(${PROJECT_NAME}
        hpr::gpu
        imgui::imgui
        implot::implot
        OpenXLSX::OpenXLSX
        )

target_link_libraries(${PROJECT_NAME} -static gcc stdc++ winpthread -dynamic)
#message(STATUS "${OpenXLSX_DIR}")
#target_include_directories(${PROJECT_NAME} PUBLIC ${OpenXLSX_DIR})