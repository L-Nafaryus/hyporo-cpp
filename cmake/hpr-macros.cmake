
# args: <file to parse> <regex>
# provide: HPR_VERSION
macro(hpr_parse_version _file _regex)
    file(
        STRINGS "${_file}" _version_defines
        REGEX "${_regex}"
    )

    set(_regex_match "${_regex} +([^ ]+)$")
    foreach(_ver ${_version_defines})
        if(_ver MATCHES ${_regex_match})
            set(_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}")
        endif()
    endforeach()

    if(_VERSION_PATCH MATCHES [[\.([a-zA-Z0-9]+)$]])
        set(_VERSION_TYPE "${CMAKE_MATCH_1}")
    endif()

    string(REGEX MATCH "^[0-9]+" _VERSION_PATCH "${_VERSION_PATCH}")
    set(HPR_VERSION "${_VERSION_MAJOR}.${_VERSION_MINOR}.${_VERSION_PATCH}")
endmacro()

# args: <project name> <paths to headers>...
# provide : *_HEADERS *_HEADERS_INTERFACE
macro(hpr_collect_interface _project_name)
    file(GLOB ${_project_name}_HEADERS ${ARGN})

    foreach(_header_path ${${_project_name}_HEADERS})
        list(APPEND ${_project_name}_HEADERS_INTERFACE "$<BUILD_INTERFACE:${_header_path}>")
    endforeach()
endmacro()

# args: <project name> <paths to sources>...
# provide : *_SOURCES
macro(hpr_collect_sources _project_name)
    file(GLOB ${_project_name}_SOURCES ${ARGN})
endmacro()

# Common installation
# args: <project name> <source dir>
macro(hpr_install _project_name _project_source_dir)
    if (HPR_INSTALL)
        install(
                TARGETS ${_project_name}
                EXPORT ${_project_name}Targets
                RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} NAMELINK_SKIP COMPONENT runtime
                ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
                INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hpr COMPONENT devel
        )

        if (BUILD_SHARED_LIBS)
            install(
                    TARGETS ${target}
                    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} NAMELINK_ONLY COMPONENT devel
            )
        endif()

        install(
                EXPORT ${_project_name}Targets
                DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hpr COMPONENT devel
                NAMESPACE hpr::
        )

        install(
                DIRECTORY ${_project_source_dir}
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hpr COMPONENT devel
                FILES_MATCHING
                PATTERN "*.h"
                PATTERN "*.hpp"
                PATTERN "tests" EXCLUDE
        )

        install(
                FILES ../${_project_name}.hpp
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/hpr COMPONENT devel
        )
    endif()
endmacro()

# Common tests template
# args: <project name> <directory with tests>
macro(hpr_tests _project_name _tests_dir)
    if (HPR_TEST)
        file(GLOB tests_cpp "${_tests_dir}/*.cpp")

        add_executable(${_project_name}-tests ${tests_cpp})

        target_link_libraries(
                ${_project_name}-tests
                PUBLIC hpr::${_project_name}
                PRIVATE GTest::gtest_main
        )

        gtest_add_tests(TARGET ${_project_name}-tests)
    endif()
endmacro()

# Collect modules
# args: -
macro(hpr_collect_modules_)
    set_property(GLOBAL PROPERTY _HPR_MODULES "")
endmacro()

# Add module library
# args: <library name> <library type>
macro(hpr_add_library _library_name _library_type)
    set_property(GLOBAL APPEND PROPERTY _HPR_MODULES "${_library_name}")
    add_library(${_library_name} ${_library_type})
    add_library(hpr::${_library_name} ALIAS ${_library_name})
endmacro()

# args: -
# provide: HPR_MODULES HPR_INTERFACE_LIBS HPR_PUBLIC_LIBS
macro(hpr_collect_modules)
    get_property(_hpr_modules GLOBAL PROPERTY _HPR_MODULES)

    foreach(_module ${_hpr_modules})
        get_target_property(_module_type ${_module} TYPE)

        if(_module_type STREQUAL "INTERFACE_LIBRARY")
            list(APPEND HPR_INTERFACE_LIBS "${PROJECT_NAME}::${_module}")
        else()
            list(APPEND HPR_PUBLIC_LIBS "${PROJECT_NAME}::${_module}")
        endif()

        set(HPR_MODULES "${HPR_MODULES} ${_module}")
    endforeach()
endmacro()

# args: -
macro(hpr_print_summary)
    set(_summary "hpr-${HPR_VERSION}:${HPR_MODULES}")
    message(STATUS ${_summary})
endmacro()
