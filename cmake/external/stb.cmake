include(${CMAKE_SOURCE_DIR}/cmake/tools/CPM.cmake)

CPMAddPackage(
        NAME stb_external
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG af1a5bc352164740c1cc1354942b1c6b72eacb8a
        DOWNLOAD_ONLY TRUE
)

if(stb_external_ADDED)
    set(EXTERNAL_PROJECT_NAME stb)

    add_library(${EXTERNAL_PROJECT_NAME} INTERFACE)
    add_library(stb::stb ALIAS ${EXTERNAL_PROJECT_NAME})

    target_include_directories(${EXTERNAL_PROJECT_NAME}
            INTERFACE
            $<BUILD_INTERFACE:${stb_external_SOURCE_DIR}>
            )

    set_target_properties(${EXTERNAL_PROJECT_NAME}
            PROPERTIES
            OUTPUT_NAME stb
            )

    include(GNUInstallDirs)

    install(
            TARGETS ${EXTERNAL_PROJECT_NAME}
            EXPORT ${EXTERNAL_PROJECT_NAME}Targets
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/${EXTERNAL_PROJECT_NAME}
    )

    install(
            EXPORT ${EXTERNAL_PROJECT_NAME}Targets
            FILE ${EXTERNAL_PROJECT_NAME}Targets.cmake
            NAMESPACE ${EXTERNAL_PROJECT_NAME}::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${EXTERNAL_PROJECT_NAME}
    )

    install(
            DIRECTORY ${stb_external_SOURCE_DIR}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${EXTERNAL_PROJECT_NAME}
            COMPONENT devel
            FILES_MATCHING
            PATTERN "*.h"
            PATTERN "*.hpp"
    )
endif()
