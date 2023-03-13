cmake_minimum_required (VERSION 3.16)

#
set(HPR_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(HPR_EXTERNAL_PATH "${HPR_MODULE_PATH}/external")
set(HPR_TOOLCHAINS_PATH "${HPR_MODULE_PATH}/toolchains")

set(HPR_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/source")

# toolchain
#if (MINGW)
#    set(CMAKE_TOOLCHAIN_FILE "${HPR_TOOLCHAINS_PATH}/mingw-gcc.cmake")
#else()
#    set(CMAKE_TOOLCHAIN_FILE "${HPR_TOOLCHAINS_PATH}/linux-gcc.cmake")
#endif()

# Extract project version from source
include(${HPR_MODULE_PATH}/hpr-macros.cmake)

hpr_parse_version("${HPR_SOURCE_DIR}/hpr/hpr.hpp" "#define HPR_VERSION_(MAJOR|MINOR|PATCH)")

# Main project
project(
    hpr
    VERSION "${HPR_VERSION}"
    LANGUAGES CXX
)

# Compiler settings
set(CMAKE_CXX_STANDARD 20)

# Detect how project is used
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
        set(lines
            "You are building in-place. If that is not what you intended to "
            "do, you can clean the source directory with:\n"
            "rm -r CMakeCache.txt CMakeFiles/ cmake_uninstall.cmake hprConfig.cmake "
            "hprConfigVersion.cmake\n")
        message(AUTHOR_WARNING ${lines})
    endif()

    set(HPR_IS_TOP_LEVEL ON)
else()
    set(HPR_IS_TOP_LEVEL OFF)
endif()

# Project options
option(HPR_INSTALL "Install hpr files?" ${HPR_IS_TOP_LEVEL})
option(HPR_TEST "Build hpr tests?" ${HPR_IS_TOP_LEVEL})

# Installation settings
include(GNUInstallDirs)

set(HPR_INSTALL_INTERFACE "$<INSTALL_INTERFACE:include/${PROJECT_NAME}") # ! '>' not included

# Package manager
include(${HPR_MODULE_PATH}/tools/CPM.cmake)

# Testing
if(HPR_TEST)
    enable_testing()

    include("${HPR_EXTERNAL_PATH}/googletest.cmake")
    include(GoogleTest)
endif()

# Uninstall target
if (NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_LIST_DIR}/cmake/templates/cmake_uninstall.cmake.in" cmake_uninstall.cmake
        IMMEDIATE @ONLY
    )

    add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake")
endif()


# Modules
option(HPR_WITH_CONTAINERS "" ON)
option(HPR_WITH_MATH "" ON)
option(HPR_WITH_IO "" ON)
option(HPR_WITH_MESH "" ON)
option(HPR_WITH_CSG "" ON)
option(HPR_WITH_GPU "" ON)
option(HPR_WITH_PARALLEL "" ON)

include_directories(${HPR_SOURCE_DIR})

add_subdirectory(source/hpr)

hpr_collect_modules()

# Main library
add_library(hpr SHARED source/hpr/hpr.cpp)
add_library(hpr::hpr ALIAS hpr)

target_sources(hpr
    INTERFACE "$<BUILD_INTERFACE:source/hpr/hpr.hpp>" "${HPR_INSTALL_INTERFACE}>"
)

target_link_libraries(hpr
    INTERFACE ${HPR_INTERFACE_LIBS}
    PUBLIC ${HPR_PUBLIC_LIBS}
)


# Installation
if (HPR_INSTALL)
    include(CMakePackageConfigHelpers)

    configure_package_config_file(
        "${HPR_MODULE_PATH}/templates/Config.cmake.in" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    write_basic_package_version_file(
        "${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion
    )

    install(FILES
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} COMPONENT devel
    )

    install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME} COMPONENT devel
    )

    install(
        EXPORT ${PROJECT_NAME}Targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} COMPONENT devel
        NAMESPACE ${CMAKE_PROJECT_NAME}::
    )
    install(
        FILES source/${PROJECT_NAME}/${PROJECT_NAME}.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME} COMPONENT devel
    )

    set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VERSION "${HPR_VERSION}")

    include(CPack)
    cpack_add_component(devel DISPLAY_NAME "Development")
    cpack_add_component(runtime DISPLAY_NAME "Runtime libraries")
endif()

# Project summary
hpr_print_summary()

# Documentation
#add_subdirectory(docs)

# Additional applications
add_subdirectory(source/creator)
add_subdirectory(source/applications)
