include(${CMAKE_SOURCE_DIR}/cmake/tools/CPM.cmake)

CPMAddPackage(
    NAME glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    #VERSION 2.0.2
    VERSION 0.1.36
    GIT_PROGRESS TRUE
    OPTIONS "GLAD_EXPORT ON" "GLAD_INSTALL ON"
)

#if(glad_ADDED)
#    add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
#    glad_add_library(glad REPRODUCIBLE API gl:core=3.3)
#endif()