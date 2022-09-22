
include_directories(
    .
    ../hyplib/integer
    ../hyplib/scalar
    ../hyplib/vector
)

add_library(hyporo-hmesh STATIC

    # Header files
    Vertex.hpp
    Edge.hpp
    Face.hpp
    Cell.hpp
    Mesh.hpp

    # Source files
    Mesh.cpp
)

if(WITH_GTESTS)
    add_executable(hyporo-hmesh-test
        tests/hmesh-test.cpp
    )

    target_link_libraries(hyporo-hmesh-test
        GTest::gtest_main
    )

    gtest_add_tests(TARGET hyporo-hmesh-test)
endif()
