
set(COMPONENT_NAME io)

add_component_library(${COMPONENT_NAME} STATIC
    file.cpp
)

#target_sources(${COMPONENT_NAME} PUBLIC
#    file.hpp
#    logger.hpp
#)

install_component_headers(${COMPONENT_NAME})
install_component_source(${COMPONENT_NAME})

if(0)
    add_executable(${COMPONENT_NAME}-test
        tests/${COMPONENT_NAME}-test.cpp
    )

    target_link_libraries(${COMPONENT_NAME}-test
        ${CMAKE_PROJECT_NAME}::${COMPONENT_NAME}
        GTest::gtest_main
    )

    gtest_add_tests(TARGET ${COMPONENT_NAME}-test)
endif()
