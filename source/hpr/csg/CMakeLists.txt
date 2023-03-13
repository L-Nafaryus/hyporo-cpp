cmake_minimum_required(VERSION 3.16)

project(csg
    VERSION "${HPR_VERSION}"
    LANGUAGES CXX
)

hpr_add_library(${PROJECT_NAME} STATIC)

hpr_collect_interface(${PROJECT_NAME}
    "../csg.hpp"
    "*.hpp"
)

hpr_collect_sources(${PROJECT_NAME}
    "*.cpp"
)

target_sources(${PROJECT_NAME}
    INTERFACE ${${PROJECT_NAME}_HEADERS_INTERFACE} ${HPR_INSTALL_INTERFACE}/${PROJECT_NAME}>
    PRIVATE ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    ${OpenCASCADE_LIBS}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${OpenCASCADE_INCLUDE_DIR}
)

hpr_install(${PROJECT_NAME} ${PROJECT_SOURCE_DIR})
hpr_tests(${PROJECT_NAME} tests)
