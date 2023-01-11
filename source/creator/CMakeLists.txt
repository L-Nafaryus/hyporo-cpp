
project(
    hyporo-creator
    VERSION 0.1.0
    LANGUAGES CXX
)

# Compiler options
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Project options
include(GNUInstallDirs)


#add_executable(${PROJECT_NAME}
#    creator.cpp
#)

include(${CMAKE_SOURCE_DIR}/cmake/external/imgui.cmake)
#message(STATUS "project name: ${PROJECT_NAME}")
#target_link_libraries(${PROJECT_NAME}
#    hpr::hpr
#    imgui
#)

#target_include_directories(${PROJECT_NAME}
#    PRIVATE
#    ../
#)

#set(CMAKE_CXX_STANDARD 20)
#add_executable(testi
#    test.cpp
#)

#target_include_directories(testi
#    PRIVATE
#    ../
#)

#target_link_libraries(testi
#    hpr::hpr
#    imgui
#)
set(CMAKE_CXX_STANDARD 20)

add_executable(hyporo-creator
        test2.cpp
        )

target_include_directories(hyporo-creator
        PRIVATE
        ../
        )

target_link_libraries(hyporo-creator
        hpr::gpu
        hpr::window-system
        imgui
        )
