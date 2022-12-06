cmake_minimum_required (VERSION 3.16)

# Dependencies
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(USE_SYSTEM_OCCT ON CACHE INTERNAL "")



# Main project
project(
    project-hyporo
    VERSION 0.10.0
    LANGUAGES CXX
)

option(WITH_GTESTS "Enable GTest unit testing" ON)

enable_testing()

if(WITH_GTESTS)
    option(INSTALL_GMOCK "" OFF)
    option(INSTALL_GTEST "" OFF)
    include(${CMAKE_SOURCE_DIR}/cmake/googletest.cmake)
    include(GoogleTest)
endif()


add_subdirectory(source)
#add_subdirectory(docs)