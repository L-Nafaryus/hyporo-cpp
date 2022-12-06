
macro(add_component_library COMPONENT LIBRARY_TYPE COMPONENT_SOURCES)
    add_library(${COMPONENT} ${LIBRARY_TYPE} ${COMPONENT_SOURCES})
    add_library(${PROJECT_NAME}::${COMPONENT} ALIAS ${COMPONENT})
endmacro()

macro(install_component_source COMPONENT)
    set_target_properties(${COMPONENT}
            PROPERTIES
            VERSION ${${PROJECT_NAME}_VERSION}
            SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR}
            OUTPUT_NAME ${PROJECT_NAME}-${COMPONENT}
            )

    install(
            TARGETS ${COMPONENT}
            EXPORT ${COMPONENT}Targets
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
    )

    install(
            EXPORT ${COMPONENT}Targets
            FILE ${COMPONENT}Targets.cmake
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
endmacro()

macro(install_component_headers COMPONENT)

    #target_include_directories(${COMPONENT}
    #        PUBLIC
    #        $<INSTALL_INTERFACE:include>
    #        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>#/${CMAKE_PROJECT_NAME}>#/include>
    #        )

    install(
            DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}#/${COMPONENT}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}#/${COMPONENT}
            COMPONENT devel
            FILES_MATCHING
            PATTERN "*.h"
            PATTERN "*.hpp"
    )

    install(
            FILES ${CMAKE_CURRENT_SOURCE_DIR}/../${COMPONENT}.hpp
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}#/${COMPONENT}
            COMPONENT devel
    )

endmacro()