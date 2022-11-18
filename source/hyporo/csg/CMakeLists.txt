
include_directories(
    .
    ../hyplib/integer
    ../hyplib/scalar
    ../hyplib/vector
)

add_library(hyporo-csg STATIC

    # Header files
        shape.hpp

    # Source files
        shape.cpp
)

target_link_libraries(hyporo-csg
        hyporo-hyplib
        ${OCCT_LIBRARIES}
        )

target_include_directories(hyporo-csg
        PUBLIC
        ${OCCT_INCLUDE_DIRS}
        )

if(WITH_GTESTS)
    add_executable(hyporo-csg-test
        tests/csg-test.cpp
            shape.cpp
    )

    target_link_libraries(hyporo-csg-test
        GTest::gtest_main
            ${OCCT_LIBRARIES}
    )

    target_include_directories(hyporo-csg-test
            PUBLIC
            ${OCCT_INCLUDE_DIRS}
            )
    gtest_add_tests(TARGET hyporo-csg-test)
endif()
