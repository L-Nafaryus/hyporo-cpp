
include_directories(
    integer
    scalar
    array
    vector
    matrix
)

add_library(hyporo-hyplib STATIC

    # Header files
    scalar/scalar.hpp

    array/iterator.hpp
    array/dynamic_array.hpp
    array/static_array.hpp
    array/array.hpp

    vector/vector_space.hpp

    vector/vector.hpp

    matrix/matrix_space.hpp

    matrix/matrix.hpp

    io/file.hpp
    io/io.hpp

    # Source files
    io/file.cpp

        matrix/matrix_space.cpp
)

if(WITH_GTESTS)
    add_executable(hyporo-hyplib-test
        tests/hyplib-test.cpp

            matrix/matrix_space.hpp
    )

    target_link_libraries(hyporo-hyplib-test

            hyporo-hyplib
        GTest::gtest_main
    )

    gtest_add_tests(TARGET hyporo-hyplib-test)

    add_executable(mytest
            tests/mytest.cpp)
endif()
