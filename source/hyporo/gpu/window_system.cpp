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

void WindowSystem::create(WindowSystem** ws, Provider provider)
{
    if (ws == nullptr)
        throw std::invalid_argument("Invalid parameter 'nullptr'");
    if (provider == Provider::Unknown)
        throw std::invalid_argument("Cannot create window system from 'Unknown' provider");

    *ws = nullptr;

    if (provider == Provider::GLFW)
        *ws = new glfw::WindowSystem;
    else
        throw std::invalid_argument("Unsupported window system");
}

void WindowSystem::destroy(WindowSystem*& ws)
{
    delete ws;
    ws = nullptr;
}

Window& WindowSystem::window(int index)
{
    return p_windows[index];
}

void WindowSystem::closeWindow(Window* window)
{
    window->close();
}

void WindowSystem::destroyWindow(Window* window)
{
    p_windows.remove(*window);
}

Monitor& WindowSystem::monitor(int index)
{
    return p_monitors[index];
}

void WindowSystem::destroyMonitor(Monitor* monitor)
{
    p_monitors.remove(*monitor);
}
}
