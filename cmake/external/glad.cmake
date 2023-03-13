
CPMAddPackage(
    NAME glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    #VERSION 2.0.2
    VERSION 0.1.36
    GIT_PROGRESS TRUE
    EXCLUDE_FROM_ALL ON
    OPTIONS "GLAD_EXPORT OFF" "GLAD_INSTALL OFF"
)

if(glad_ADDED)
    set(EXTERNAL_PROJECT_NAME glad)

    include(GNUInstallDirs)

    add_library(glad::glad ALIAS glad)

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
            DIRECTORY ${glad_BINARY_DIR}/include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            COMPONENT devel
            FILES_MATCHING
            PATTERN "*.h"
            PATTERN "*.hpp"
    )

endif()