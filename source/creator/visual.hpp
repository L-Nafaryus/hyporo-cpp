#pragma once

#include <hpr/gpu.hpp>
#include "shaders.hpp"

using namespace hpr;

class Visual
{

protected:

    gpu::Window* p_window;
    gpu::ShaderProgram* p_shaderProgram;
    gpu::ColorBuffer p_colorBuffer;
    gpu::DepthBuffer p_depthBuffer;

public:

    Visual() :
        p_window {new gpu::Window{50, 50, 1000, 800, "Hyporo", gpu::Window::Windowed, nullptr, nullptr}},
        p_shaderProgram {new gpu::ShaderProgram},
        p_colorBuffer {},
        p_depthBuffer {}
    {
        gpu::Shader vertexShader {gpu::Shader::Type::Vertex, vertexSource};
        vertexShader.create();

        gpu::Shader fragmentShader {gpu::Shader::Type::Fragment, fragmentSource};
        fragmentShader.create();

        p_shaderProgram->create();
        p_shaderProgram->attach(vertexShader);
        p_shaderProgram->attach(fragmentShader);
        p_shaderProgram->link();

        vertexShader.destroy();
        fragmentShader.destroy();

        p_window->framebufferResizeCallback([](gpu::Window* w, int width, int height){
            w->size(width, height);
        });
    }

    ~Visual()
    {
        p_shaderProgram->destroy();
        delete p_shaderProgram;
        p_window->destroy();
        delete p_window;
    }

    gpu::Window* window()
    {
        return p_window;
    }

    gpu::ShaderProgram* shaderProgram()
    {
        return p_shaderProgram;
    }

    gpu::ColorBuffer& colorBuffer()
    {
        return p_colorBuffer;
    }

    gpu::DepthBuffer& depthBuffer()
    {
        return p_depthBuffer;
    }

    void render()
    {
        p_window->swapBuffers();
        p_window->pollEvents();
    }
};