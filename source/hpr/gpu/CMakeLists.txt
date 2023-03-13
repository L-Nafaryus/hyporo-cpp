cmake_minimum_required(VERSION 3.16)

project(gpu
    VERSION "${HPR_VERSION}"
    LANGUAGES CXX
)

hpr_add_library(${PROJECT_NAME} INTERFACE)

hpr_collect_interface(${PROJECT_NAME}
    "../gpu.hpp"
    "*.hpp"
)

#hpr_collect_sources(${PROJECT_NAME}
#    "*.cpp"
#)

target_sources(${PROJECT_NAME}
    INTERFACE ${${PROJECT_NAME}_HEADERS_INTERFACE} ${HPR_INSTALL_INTERFACE}/${PROJECT_NAME}>
    PRIVATE ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
        INTERFACE
    glad::glad
    glfw::glfw
    stb::stb
)


hpr_install(${PROJECT_NAME} ${PROJECT_SOURCE_DIR})
