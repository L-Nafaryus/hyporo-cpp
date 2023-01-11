cmake_minimum_required (VERSION 3.16)

# Extract project version from source
file(STRINGS "source/hpr/core/common.hpp"
        hpr_version_defines REGEX "#define HPR_VERSION_(MAJOR|MINOR|PATCH) ")

foreach(ver ${hpr_version_defines})
    if(ver MATCHES [[#define HPR_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$]])
        set(HPR_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}")
    endif()
endforeach()

if(HPR_VERSION_PATCH MATCHES [[\.([a-zA-Z0-9]+)$]])
    set(hpr_VERSION_TYPE "${CMAKE_MATCH_1}")
endif()

string(REGEX MATCH "^[0-9]+" HPR_VERSION_PATCH "${HPR_VERSION_PATCH}")
set(HPR_PROJECT_VERSION "${HPR_VERSION_MAJOR}.${HPR_VERSION_MINOR}.${HPR_VERSION_PATCH}")

# Main project
project(
    hpr
    VERSION "${HPR_PROJECT_VERSION}"
    LANGUAGES CXX
)

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

    set(HPR_MASTER_PROJECT ON)
    set(CMAKE_CXX_STANDARD 20)

    message(STATUS ${CMAKE_SOURCE_DIR})
else()
    set(HPR_MASTER_PROJECT OFF)
endif()



# Standard includes
include(GNUInstallDirs)
include(CPack)

# Options
option(HPR_INSTALL "Install hpr files?" ${HPR_MASTER_PROJECT})
option(HPR_TEST "Build hpr tests?" ${HPR_MASTER_PROJECT})

# Testing
if(HPR_TEST)
    enable_testing()

    option(INSTALL_GMOCK "" OFF)
    option(INSTALL_GTEST "" OFF)

    include(${CMAKE_SOURCE_DIR}/cmake/external/googletest.cmake)
    include(GoogleTest)
endif()

# Modules
option(WITH_CONTAINERS "" ON)
option(WITH_MATH "" ON)
option(WITH_IO "" ON)
option(WITH_MESH "" ON)
option(WITH_CSG "" ON)
    option(USE_SYSTEM_OCCT "" ON)
option(WITH_GPU "" ON)
option(WITH_WINDOW_SYSTEM "" ON)

set_property(GLOBAL PROPERTY HPR_MODULES "")
macro(add_module MODULE)
    set_property(GLOBAL APPEND PROPERTY HPR_MODULES "${MODULE}")
endmacro()

add_subdirectory(source/hpr)

get_property(hpr_modules GLOBAL PROPERTY HPR_MODULES)
foreach(module ${hpr_modules})
    get_target_property(module_type ${module} TYPE)
    if(module_type STREQUAL "INTERFACE_LIBRARY")
        list(APPEND HPR_INTERFACE_LIBS "${PROJECT_NAME}::${module}")
    else()
        list(APPEND HPR_PUBLIC_LIBS "${PROJECT_NAME}::${module}")
    endif()
    set(hpr_modules_summary "${hpr_modules_summary} ${module}")
endforeach()

# Main library
add_library(hpr SHARED
    source/hpr/hpr.cpp)
add_library(hpr::hpr ALIAS hpr)

target_sources(hpr
    INTERFACE
        "$<BUILD_INTERFACE:source/hpr/hpr.hpp>"
        "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>"
)

target_link_libraries(hpr
    INTERFACE
        ${HPR_INTERFACE_LIBS}
    PUBLIC
        ${HPR_PUBLIC_LIBS}
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
)

write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
)
install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        NAMELINK_SKIP
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(
        EXPORT ${PROJECT_NAME}Targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}-${HPR_PROJECT_VERSION}
        NAMESPACE ${CMAKE_PROJECT_NAME}::
)
install(
        TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
        NAMELINK_ONLY
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Project summary
set(summary
    "Summary:\n"
    "Version: ${HPR_PROJECT_VERSION}\n"
    "Master: ${HPR_MASTER_PROJECT}\n"
    "Modules: ${hpr_modules_summary}"
)
message(STATUS ${summary})

# Documentation
#add_subdirectory(docs)

# Additional applications
add_subdirectory(source/applications)
add_subdirectory(source/creator)