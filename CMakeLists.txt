cmake_minimum_required (VERSION 3.16)


include(${CMAKE_SOURCE_DIR}/cmake/glad.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/imgui.cmake)

project(
    hyporo
    VERSION 0.20.0
    LANGUAGES CXX
)

option(WITH_GTESTS "Enable GTest unit testing" ON)

enable_testing()

if(WITH_GTESTS)
    include(${CMAKE_SOURCE_DIR}/cmake/googletest.cmake)
    include(GoogleTest)
endif()



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
add_definitions(-DPRECISION_FLOAT)

add_subdirectory(source)

