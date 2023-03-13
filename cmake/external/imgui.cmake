
# branch: docking
CPMAddPackage(
        NAME imgui_external
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG 24dfebf455ac1f7685e1a72272d37b72601fe70c
        DOWNLOAD_ONLY YES
)

if(imgui_external_ADDED)
    set(EXTERNAL_PROJECT_NAME imgui)

    set(CMAKE_CXX_STANDARD 17)

    find_package(PkgConfig REQUIRED)
    find_package(Freetype REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(GLFW REQUIRED)

    add_library(${EXTERNAL_PROJECT_NAME}
            ${imgui_external_SOURCE_DIR}/imgui.cpp
            ${imgui_external_SOURCE_DIR}/imgui_demo.cpp
            ${imgui_external_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_external_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_external_SOURCE_DIR}/imgui_widgets.cpp

            ${imgui_external_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
            ${imgui_external_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp

            ${imgui_external_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp
            ${imgui_external_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
            )
    add_library(imgui::imgui ALIAS ${EXTERNAL_PROJECT_NAME})

    add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)

    target_include_directories(${EXTERNAL_PROJECT_NAME}
            PUBLIC
            $<BUILD_INTERFACE:${imgui_external_SOURCE_DIR}>
            $<BUILD_INTERFACE:${imgui_external_SOURCE_DIR}/backends>
            $<BUILD_INTERFACE:${imgui_external_SOURCE_DIR}/misc/freetype>
            $<BUILD_INTERFACE:${imgui_external_SOURCE_DIR}/misc/cpp>

            ${FREETYPE_INCLUDE_DIRS}
            ${GLFW_INCLUDE_DIRS}
            )

    target_link_libraries(${EXTERNAL_PROJECT_NAME}
            PUBLIC
            Freetype::Freetype
            glfw
            OpenGL::GL
            )

    set_target_properties(${EXTERNAL_PROJECT_NAME}
            PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            OUTPUT_NAME imgui
            EXCLUDE_FROM_ALL ON
            )

    if(IMGUI_INSTALL)
        include(GNUInstallDirs)

        install(
                TARGETS ${EXTERNAL_PROJECT_NAME}
                EXPORT ${EXTERNAL_PROJECT_NAME}Targets
                RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
                COMPONENT runtime
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
                COMPONENT devel
                ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
                COMPONENT devel
                INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
                COMPONENT devel

        )

        install(
                EXPORT ${EXTERNAL_PROJECT_NAME}Targets
                FILE ${EXTERNAL_PROJECT_NAME}Targets.cmake
                NAMESPACE ${EXTERNAL_PROJECT_NAME}::
                DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${EXTERNAL_PROJECT_NAME}
                COMPONENT devel
        )

        install(
                DIRECTORY ${imgui_external_SOURCE_DIR}/
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${EXTERNAL_PROJECT_NAME}
                COMPONENT devel
                FILES_MATCHING
                PATTERN "*.h"
                PATTERN "*.hpp"
        )
    endif()
endif()
