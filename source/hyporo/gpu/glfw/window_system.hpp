#pragma once

#include "../window_system.hpp"

#include <GLFW/glfw3.h>
#include <functional>


namespace hpr::gpu::glfw
{

class WindowSystem : public gpu::WindowSystem
{

public:

    WindowSystem();

    ~WindowSystem() override;

    gpu::Window* newWindow() override;

    std::function<GLFWglproc(const char*)> deviceProcAddress() const;
};

}