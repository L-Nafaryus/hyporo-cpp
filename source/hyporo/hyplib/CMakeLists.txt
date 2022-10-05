
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
    io/file.hpp
    io/io.hpp

    # Source files
    scalar/scalar.cpp
    io/file.cpp
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
