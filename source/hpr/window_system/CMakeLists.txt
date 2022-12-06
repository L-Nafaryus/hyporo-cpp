
set(COMPONENT_NAME window_system)

add_component_library(${COMPONENT_NAME} OBJECT
    window_system.cpp
    monitor.cpp
    window.cpp
    glfw/window_system.cpp
    glfw/monitor.cpp
    glfw/window.cpp
)

#target_sources(${COMPONENT_NAME} PUBLIC

#)

target_link_libraries(${COMPONENT_NAME}
    glfw
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