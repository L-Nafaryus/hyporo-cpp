#pragma once

#include <GLFW/glfw3.h>


namespace hpr::gpu
{

class Monitor
{

protected:

    GLFWmonitor* p_instance;

public:

    inline
    Monitor() :
        p_instance {glfwGetPrimaryMonitor()}
    {}

    virtual
    ~Monitor() = default;

    inline
    GLFWmonitor* instance() const
    {
        return p_instance;
    }

};

}