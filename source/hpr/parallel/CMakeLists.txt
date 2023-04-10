cmake_minimum_required(VERSION 3.16)

project(parallel
    VERSION "${HPR_VERSION}"
    LANGUAGES CXX
)

hpr_add_library(${PROJECT_NAME} INTERFACE)

hpr_collect_interface(${PROJECT_NAME}
    "../parallel.hpp"
    "*.hpp"
)

target_sources(${PROJECT_NAME}
    INTERFACE ${${PROJECT_NAME}_HEADERS_INTERFACE} ${HPR_INSTALL_INTERFACE}/${PROJECT_NAME}>
)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        tbb
)

hpr_install(${PROJECT_NAME} ${PROJECT_SOURCE_DIR})
