
set(COMPONENT_NAME csg)

add_component_library(${COMPONENT_NAME} STATIC
    shape.cpp
)

#target_sources(${COMPONENT_NAME} PUBLIC

#)

target_link_libraries(${COMPONENT_NAME}
    ${OCCT_LIBRARIES}
)

target_include_directories(${COMPONENT_NAME}
    PUBLIC
    ${OCCT_INCLUDE_DIRS}
)

install_component_headers(${COMPONENT_NAME})
install_component_source(${COMPONENT_NAME})

if (0)
    add_executable(${COMPONENT_NAME}-test
        tests/${COMPONENT_NAME}-test.cpp
    )

    target_link_libraries(${COMPONENT_NAME}-test
        ${PROJECT_NAME}::${COMPONENT_NAME}
        GTest::gtest_main
    )

    gtest_add_tests(TARGET ${COMPONENT_NAME}-test)
endif()
