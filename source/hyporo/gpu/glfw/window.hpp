#pragma once

#include "../window.hpp"

#include <GLFW/glfw3.h>


namespace hpr::gpu::glfw
{

class Window : public gpu::Window
{
    friend class WindowSystem;

protected:

    GLFWwindow* p_instance;

public:

    Window();

    virtual
    ~Window();

    GLFWwindow* instance() const
    {
        return p_instance;
    }
    void init(const std::string& title, Style style, int x, int y, int width, int height, gpu::Window* parent, gpu::Monitor* monitor) override;

    void init(const std::string& title, Style style, gpu::Window* parent, gpu::Monitor* monitor) override;

    void state(State state) override;

    void close() override;

    void style(Style style) override;

    void swapBuffers();

    void pollEvents();
};

}