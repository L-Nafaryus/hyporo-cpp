
include_directories(
    .
    ../hyplib/integer
    ../hyplib/scalar
    ../hyplib/vector
)

add_library(hyporo-mesh STATIC

    # Header files
        mesh.hpp
        vertex.hpp

    # Source files
        mesh.cpp
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
