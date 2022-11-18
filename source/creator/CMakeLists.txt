
include_directories(
    ../hyporo/hyplib
    ../hyporo/window_system
    ../hyporo/gpu
    ../hyporo/hmesh
)

add_executable(hyporo
    # Source files
    creator.cpp

)

target_link_libraries(hyporo
    PUBLIC
    hyporo-hyplib
    hyporo-window-system
    hyporo-gpu
    hyporo-mesh
    imgui
)
