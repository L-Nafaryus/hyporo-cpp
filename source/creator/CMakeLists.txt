
include_directories(
    ../hyporo/hyplib/vector
    ../hyporo/hyplib/scalar
    ../hyporo/hyplib/integer
    ../hyporo/hyplib/matrix
)

add_executable(hyporo
    # Source files
    creator.cpp

)

target_link_libraries(hyporo
    PUBLIC
    hyporo-gpu
        imgui)
