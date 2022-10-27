#pragma once

#include "../window_system.hpp"

#include <GLFW/glfw3.h>


namespace hpr::gpu::glfw
{

class WindowSystem : public gpu::WindowSystem
{

protected:

    GLFWwindow p_instance;

public:

    WindowSystem();

    ~WindowSystem();

    virtual
    void newWindow(Window** window);
};

}