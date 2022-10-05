include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

CPMAddPackage(
    NAME glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    VERSION 0.1.36
    GIT_PROGRESS TRUE
    OPTIONS "GLAD_EXPORT ON" "GLAD_INSTALL ON"
)
