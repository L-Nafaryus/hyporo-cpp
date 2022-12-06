
set(COMPONENT_NAME containers)

add_component_library(${COMPONENT_NAME} INTERFACE ""
)

#target_sources(${COMPONENT_NAME} PUBLIC
#    array/dynamic_array.hpp
#    array.hpp
#    array/iterator.hpp
#    array/static_array.hpp
#)

install_component_headers(${COMPONENT_NAME})
install_component_source(${COMPONENT_NAME})

if (WITH_GTEST)
    add_executable(${COMPONENT_NAME}-test
        tests/${COMPONENT_NAME}-test.cpp
    )

    target_link_libraries(${COMPONENT_NAME}-test
        ${CMAKE_PROJECT_NAME}::${COMPONENT_NAME}
        GTest::gtest_main
    )

    gtest_add_tests(TARGET ${COMPONENT_NAME}-test)
endif()
