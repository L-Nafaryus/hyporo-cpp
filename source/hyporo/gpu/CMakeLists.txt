
include_directories(
    ../hyplib
)

add_library(hyporo-gpu STATIC
    # Source files
        window_system.cpp
        monitor.cpp
        buffer.cpp
        context.cpp
        shader.cpp
        window.cpp
        opengl/buffer.cpp
        opengl/context.cpp
        opengl/shader.cpp
        opengl/texture.cpp
        #opengl/device.cpp
        opengl/shader_program.cpp
        texture.cpp
        #device.cpp
        glfw/window_system.cpp
        glfw/monitor.cpp
        glfw/window.cpp
        shader_program.cpp

    # Header files
        monitor.hpp
        context.hpp
        shader.hpp
        window_context.hpp
        shader_program.hpp
        buffer.hpp
        #device.hpp
        window.hpp
        texture.hpp
        opengl/context.hpp
        opengl/shader.hpp
        opengl/shader_program.hpp
        opengl/buffer.hpp
        #opengl/device.hpp
        opengl/texture.hpp
        window_system.hpp
        glfw/monitor.hpp
        glfw/window.hpp
        glfw/window_system.hpp
)

target_link_libraries(hyporo-gpu
    glad
    glfw
    hyporo-hyplib
)