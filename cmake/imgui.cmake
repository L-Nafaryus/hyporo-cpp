include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

# branch: docking
CPMAddPackage(
        NAME imgui_external
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG 24dfebf455ac1f7685e1a72272d37b72601fe70c
        DOWNLOAD_ONLY YES
)

if(imgui_external_ADDED)
    project(imgui)

    set(CMAKE_CXX_STANDARD 17)

    find_package(PkgConfig REQUIRED)
    find_package(Freetype REQUIRED)
    find_package(OpenGL REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)

    add_library(${PROJECT_NAME} STATIC
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
    add_library(imgui::imgui ALIAS ${PROJECT_NAME})

    add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)

    target_include_directories(${PROJECT_NAME}
            PUBLIC
            $<BUILD_INTERFACE:${imgui_external_SOURCE_DIR}>
            $<BUILD_INTERFACE:${imgui_external_SOURCE_DIR}/backends>
            $<BUILD_INTERFACE:${imgui_external_SOURCE_DIR}/misc/freetype>
            $<BUILD_INTERFACE:${imgui_external_SOURCE_DIR}/misc/cpp>

            ${FREETYPE_INCLUDE_DIRS}
            ${GLFW_INCLUDE_DIRS}
            )

    target_link_libraries(${PROJECT_NAME}
            PUBLIC
            Freetype::Freetype
            glfw
            OpenGL::GL
            )

    set_target_properties(${PROJECT_NAME}
            PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            OUTPUT_NAME imgui
            EXCLUDE_FROM_ALL ON
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
            DIRECTORY ${imgui_external_SOURCE_DIR}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
            COMPONENT devel
            FILES_MATCHING
            PATTERN "*.h"
            PATTERN "*.hpp"
    )
endif()
