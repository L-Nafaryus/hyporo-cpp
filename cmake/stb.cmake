include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

CPMAddPackage(
        NAME stb_external
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG af1a5bc352164740c1cc1354942b1c6b72eacb8a
        DOWNLOAD_ONLY TRUE
)

if(stb_external_ADDED)
    project(stb)

    add_library(${PROJECT_NAME} INTERFACE)
    add_library(stb::stb ALIAS ${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME}
            INTERFACE
            $<BUILD_INTERFACE:${stb_external_SOURCE_DIR}>
            )

    set_target_properties(${PROJECT_NAME}
            PROPERTIES
            OUTPUT_NAME stb
            )

    include(GNUInstallDirs)

    install(
            TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Targets
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
    )

    install(
            EXPORT ${PROJECT_NAME}Targets
            FILE ${PROJECT_NAME}Targets.cmake
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    install(
            DIRECTORY ${stb_external_SOURCE_DIR}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
            COMPONENT devel
            FILES_MATCHING
            PATTERN "*.h"
            PATTERN "*.hpp"
    )
endif()
