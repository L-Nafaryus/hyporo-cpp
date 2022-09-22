
include_directories(
    integer
    scalar
    vector
    matrix
)

add_library(hyporo-hyplib STATIC

    # Header files
    integer/integer.hpp
    scalar/scalar.hpp
    vector/VectorSpace.hpp
    vector/vector.hpp
    matrix/MatrixSpace.hpp
    matrix/matrix.hpp

    # Source files
    scalar/scalar.cpp
)

if(WITH_GTESTS)
    add_executable(hyporo-hyplib-test
        tests/hyplib-test.cpp
    )

    target_link_libraries(hyporo-hyplib-test
        GTest::gtest_main
    )

    gtest_add_tests(TARGET hyporo-hyplib-test)
endif()
