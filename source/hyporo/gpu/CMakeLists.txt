
include_directories(
    ../hyplib
)

add_library(hyporo-gpu STATIC
    # Source files
        buffer.cpp
        context.cpp
        device.cpp
        opengl/buffer.cpp
        opengl/context.cpp
        opengl/device.cpp
        opengl/shader.cpp
        opengl/shader_program.cpp
        opengl/texture.cpp
        shader.cpp
        shader_program.cpp
        texture.cpp

    # Header files
        context.hpp
        shader.hpp
        shader_program.hpp
        buffer.hpp
        device.hpp
        texture.hpp
        opengl/context.hpp
        opengl/shader.hpp
        opengl/shader_program.hpp
        opengl/buffer.hpp
        opengl/device.hpp
        opengl/texture.hpp

)

target_link_libraries(hyporo-gpu
    glad
    stb
    hyporo-hyplib
)