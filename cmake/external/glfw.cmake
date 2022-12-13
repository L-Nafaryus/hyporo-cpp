include(${CMAKE_SOURCE_DIR}/cmake/tools/CPM.cmake)

CPMAddPackage(
        NAME glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.7
        EXCLUDE_FROM_ALL ON
)