#include "window_system.hpp"
#include "glfw/window_system.hpp"


namespace hpr::gpu
{

WindowSystem::WindowSystem() :
    WindowContext {Provider::Unknown}
{}

WindowSystem::WindowSystem(Provider provider) :
    WindowContext {provider}
{}

WindowSystem::~WindowSystem() = default;

WindowSystem* WindowSystem::create(Provider provider)
{
    if (provider == Provider::Unknown)
        throw std::invalid_argument("Cannot create window system from 'Unknown' provider");

    if (provider == Provider::GLFW)
        return new glfw::WindowSystem;
    else
        throw std::invalid_argument("Unsupported window system");
}

void WindowSystem::destroy(WindowSystem*& ws)
{
    delete ws;
    ws = nullptr;
}

Window* WindowSystem::window(int index)
{
    return p_windows[index];
}

void WindowSystem::closeWindow(Window* window)
{
    window->close();
}

void WindowSystem::destroyWindow(Window* window)
{
    for (auto n = 0; n < p_windows.size(); ++n)
        if (p_windows[n] == window)
            p_windows.remove(n);
    window->close();
    window = nullptr;
}

Monitor* WindowSystem::monitor(int index)
{
    return p_monitors[index];
}

void WindowSystem::destroyMonitor(Monitor* monitor)
{
    for (auto n = 0; n < p_monitors.size(); ++n)
        if (p_monitors[n] == monitor)
            p_monitors.remove(n);
}
}
