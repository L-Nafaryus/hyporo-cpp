cmake_minimum_required (VERSION 3.16)


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


include(${CMAKE_SOURCE_DIR}/cmake/glad.cmake)
add_definitions(-DPRECISION_FLOAT)

add_subdirectory(source)

