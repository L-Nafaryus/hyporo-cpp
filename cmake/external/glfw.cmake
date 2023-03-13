
CPMAddPackage(
        NAME glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.7
        #EXCLUDE_FROM_ALL ON
        OPTIONS "GLFW_INSTALL OFF" "GLFW_BUILD_EXAMPLES OFF" "GLFW_BUILD_TESTS OFF"
)

if(glad_ADDED)
    set(EXTERNAL_PROJECT_NAME glfw)

    include(GNUInstallDirs)

    add_library(glfw::glfw ALIAS glfw)

    install(DIRECTORY "${glfw_SOURCE_DIR}/include/GLFW" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            COMPONENT devel
            FILES_MATCHING PATTERN glfw3.h PATTERN glfw3native.h)

    install(FILES "${glfw_BINARY_DIR}/src/glfw3Config.cmake"
            "${glfw_BINARY_DIR}/src/glfw3ConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/glfw3
            COMPONENT devel)

    install(EXPORT glfwTargets FILE glfw3Targets.cmake
            EXPORT_LINK_INTERFACE_LIBRARIES
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/glfw3
            COMPONENT devel)

    install(FILES "${glfw_BINARY_DIR}/src/glfw3.pc"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
            COMPONENT devel)
    install(
            TARGETS ${EXTERNAL_PROJECT_NAME}
            EXPORT ${EXTERNAL_PROJECT_NAME}Targets
            RUNTIME COMPONENT runtime
            LIBRARY COMPONENT devel
            ARCHIVE COMPONENT devel
            INCLUDES COMPONENT devel
    )

endif()
