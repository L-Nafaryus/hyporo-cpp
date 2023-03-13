
CPMAddPackage(
        NAME implot_external
        GIT_REPOSITORY https://github.com/epezent/implot.git
        GIT_TAG v0.14
        DOWNLOAD_ONLY YES
)

if(implot_external_ADDED)
    set(EXTERNAL_PROJECT_NAME implot)

    set(CMAKE_CXX_STANDARD 17)

    find_package(PkgConfig REQUIRED)
    find_package(Freetype REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(GLFW REQUIRED)

    add_library(${EXTERNAL_PROJECT_NAME}
            ${implot_external_SOURCE_DIR}/implot.cpp
            ${implot_external_SOURCE_DIR}/implot_demo.cpp
            ${implot_external_SOURCE_DIR}/implot_items.cpp
            )
    add_library(implot::implot ALIAS ${EXTERNAL_PROJECT_NAME})


    target_include_directories(${EXTERNAL_PROJECT_NAME}
            PUBLIC
            $<BUILD_INTERFACE:${implot_external_SOURCE_DIR}>
            )

    target_link_libraries(${EXTERNAL_PROJECT_NAME}
            PUBLIC
            imgui::imgui
            )

    set_target_properties(${EXTERNAL_PROJECT_NAME}
            PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            OUTPUT_NAME implot
            EXCLUDE_FROM_ALL ON
            )

    if(IMPLOT_INSTALL)
        include(GNUInstallDirs)

        install(
                TARGETS ${EXTERNAL_PROJECT_NAME}
                EXPORT ${EXTERNAL_PROJECT_NAME}Targets
                RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
                ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
                INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT devel
        )

        install(
                EXPORT ${EXTERNAL_PROJECT_NAME}Targets
                FILE ${EXTERNAL_PROJECT_NAME}Targets.cmake
                NAMESPACE ${EXTERNAL_PROJECT_NAME}::
                DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${EXTERNAL_PROJECT_NAME}
                COMPONENT devel
        )

        install(
                DIRECTORY ${implot_external_SOURCE_DIR}/
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${EXTERNAL_PROJECT_NAME}
                COMPONENT devel
                FILES_MATCHING
                PATTERN "*.h"
                PATTERN "*.hpp"
        )
    endif()
endif()
