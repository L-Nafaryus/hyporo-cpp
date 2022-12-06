#pragma once

#include "../monitor.hpp"

#include <GLFW/glfw3.h>


namespace hpr::gpu::glfw
{

class Monitor : public gpu::Monitor
{
    friend class WindowSystem;
    friend class Window;

protected:

    GLFWmonitor* p_instance;

public:

    Monitor();

    ~Monitor();

};

}