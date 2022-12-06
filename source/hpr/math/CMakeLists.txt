
set(COMPONENT_NAME math)

add_component_library(${COMPONENT_NAME} STATIC
    vector/vector_space.cpp
    matrix/matrix_space.cpp
)

#target_sources(${COMPONENT_NAME} PUBLIC
#    matrix/clip_space.hpp
#    matrix.hpp
#    matrix/matrix_space.hpp
#    matrix/transform.hpp
#    scalar.hpp
#    scalar/scalar.hpp
#    vector.hpp
#    vector/vector_space.hpp
#)

install_component_headers(${COMPONENT_NAME})
install_component_source(${COMPONENT_NAME})

if(WITH_GTESTS)
    add_executable(${COMPONENT_NAME}-test
        tests/${COMPONENT_NAME}-test.cpp
    )

    target_link_libraries(${COMPONENT_NAME}-test
        ${PROJECT_NAME}::${COMPONENT_NAME}
        GTest::gtest_main
    )

    gtest_add_tests(TARGET ${COMPONENT_NAME}-test)
endif()
