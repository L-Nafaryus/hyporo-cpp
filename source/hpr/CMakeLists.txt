
project(
    hpr
    VERSION 0.10.0
    LANGUAGES CXX
)

# Compiler options
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_definitions(-DPRECISION_DOUBLE)

# Project options
include(GNUInstallDirs)
include(${CMAKE_SOURCE_DIR}/cmake/macros.cmake)

option(WITH_CSG "Build with CSG component" ON)
option(WITH_GPU "Build with graphics component" ON)
option(WITH_MESH "Build with mesh utils component" ON)
option(WITH_WS "Build with window system component" ON)

# containers
file(GLOB containers_hpp "containers.hpp" "containers/*.hpp" "containers/array/*.hpp")
file(GLOB containers_cpp "containers/*.cpp" "containers/array/*.cpp")

# math
file(GLOB math_hpp "math.hpp" "math/*.hpp" "math/scalar/*.hpp" "math/vector/*.hpp" "math/matrix/*.hpp")
file(GLOB math_cpp "math/*.cpp" "math/scalar/*.cpp" "math/vector/*.cpp" "math/matrix/*.cpp")

# io
file(GLOB io_hpp "io.hpp" "io/*.hpp")
file(GLOB io_cpp "io/*.cpp")

# csg
file(GLOB csg_hpp "csg.hpp" "csg/*.hpp")
file(GLOB csg_cpp "csg/*.cpp")

# gpu
file(GLOB gpu_hpp "gpu.hpp" "gpu/*.hpp")
file(GLOB gpu_cpp "gpu/*.cpp")
file(GLOB gpu_opengl_hpp "gpu/opengl/*.hpp")
file(GLOB gpu_opengl_cpp "gpu/opengl/*.cpp")

# mesh
file(GLOB mesh_hpp "mesh.hpp" "mesh/*.hpp")
file(GLOB mesh_cpp "mesh/*.cpp")

# window system
file(GLOB window_system_hpp "window_system.hpp" "window_system/*.hpp")
file(GLOB window_system_cpp "window_system/*.cpp")
file(GLOB window_system_glfw_hpp "window_system/glfw/*.hpp")
file(GLOB window_system_glfw_cpp "window_system/glfw/*.cpp")

#
list(APPEND hpr_INCLUDE
    ${containers_hpp}
    ${math_hpp}
    ${io_hpp}
)
list(APPEND hpr_SOURCES
    ${containers_cpp}
    ${math_cpp}
    ${io_cpp}
)
list(APPEND hpr_LIBRARIES "")
list(APPEND hpr_INCLUDE_DIRS "")

if (WITH_CSG)
    include(${CMAKE_SOURCE_DIR}/cmake/occt.cmake)

    list(APPEND hpr_INCLUDE ${csg_hpp})
    list(APPEND hpr_SOURCES ${csg_cpp})
    list(APPEND hpr_LIBRARIES ${OCCT_LIBRARIES})

    list(APPEND hpr_INCLUDE_DIRS ${OCCT_INCLUDE_DIRS})
endif()

if (WITH_GPU)
    include(${CMAKE_SOURCE_DIR}/cmake/glad.cmake)
    include(${CMAKE_SOURCE_DIR}/cmake/stb.cmake)

    list(APPEND hpr_INCLUDE ${gpu_hpp} ${gpu_opengl_hpp})
    list(APPEND hpr_SOURCES ${gpu_cpp} ${gpu_opengl_cpp})
    list(APPEND hpr_LIBRARIES glad stb)
endif()

if (WITH_WS)
    list(APPEND hpr_INCLUDE ${window_system_hpp} ${window_system_glfw_hpp})
    list(APPEND hpr_SOURCES ${window_system_cpp} ${window_system_glfw_cpp})
    list(APPEND hpr_LIBRARIES glfw)
endif()

add_library(${PROJECT_NAME} SHARED
    hpr.cpp
    ${hpr_SOURCES}
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    ${hpr_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${hpr_INCLUDE_DIRS}
)

# Install library

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
)

install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

include(CPack)

#
message(STATUS "Components:")
message(STATUS "    Core: ON")
message(STATUS "    CSG: ${WITH_CSG}")
message(STATUS "    GPU: ${WITH_GPU}")
message(STATUS "    Mesh: ${WITH_MESH}")
message(STATUS "    WindowSystem: ${WITH_WS}")
