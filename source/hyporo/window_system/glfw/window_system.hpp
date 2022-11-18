#pragma once

#include "../window_system.hpp"


namespace hpr::gpu::glfw
{

class WindowSystem : public gpu::WindowSystem
{

public:

    WindowSystem();

    ~WindowSystem() override;

    gpu::Window* newWindow() override;

};

}