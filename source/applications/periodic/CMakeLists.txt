
add_executable(periodic
    periodic.cpp
)

target_link_libraries(periodic
    hyporo-hyplib
    hyporo-csg
)

