cmake_minimum_required(VERSION 3.16)

project(mesh
        VERSION "${HPR_VERSION}"
        LANGUAGES CXX
        )

hpr_add_library(${PROJECT_NAME} STATIC)

hpr_collect_interface(${PROJECT_NAME}
    "../mesh.hpp"
    "*.hpp"
)

hpr_collect_sources(${PROJECT_NAME}
    "*.cpp"
)

target_sources(${PROJECT_NAME}
    INTERFACE ${${PROJECT_NAME}_HEADERS_INTERFACE} ${HPR_INSTALL_INTERFACE}/${PROJECT_NAME}>
    PRIVATE ${${PROJECT_NAME}_SOURCES}
)


hpr_install(${PROJECT_NAME} ${PROJECT_SOURCE_DIR})
hpr_tests(${PROJECT_NAME} tests)



