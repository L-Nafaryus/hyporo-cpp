
set(COMPONENT_NAME gpu)

add_component_library(${COMPONENT_NAME} STATIC
    buffer.cpp
    context.cpp
    device.cpp
    shader.cpp
    shader_program.cpp
    texture.cpp
    opengl/buffer.cpp
    opengl/context.cpp
    opengl/device.cpp
    opengl/shader.cpp
    opengl/shader_program.cpp
    opengl/texture.cpp
)

#target_sources(${COMPONENT_NAME} PUBLIC

#)

target_link_libraries(${COMPONENT_NAME}
    glad
    stb
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