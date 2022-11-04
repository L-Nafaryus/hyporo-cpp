
include_directories(
    ../hyplib
)

add_library(hyporo-window-system STATIC
    # Source files
        window_system.cpp
        monitor.cpp
        window.cpp
        glfw/window_system.cpp
        glfw/monitor.cpp
        glfw/window.cpp

    # Header files
        monitor.hpp
        window_context.hpp
        window.hpp
        window_system.hpp
        glfw/monitor.hpp
        glfw/window.hpp
        glfw/window_system.hpp
)

target_link_libraries(hyporo-window-system
    glfw
    hyporo-hyplib
)