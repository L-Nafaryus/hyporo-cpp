file(GLOB tests_cpp "*.cpp")

add_executable(${PROJECT_NAME}-tests
    ${tests_cpp}
)

target_link_libraries(${PROJECT_NAME}-tests
    PUBLIC
        hpr::${PROJECT_NAME}
    PRIVATE
        GTest::gtest_main
)

gtest_add_tests(TARGET ${PROJECT_NAME}-tests)
