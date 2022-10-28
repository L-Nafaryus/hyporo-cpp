#include "monitor.hpp"
#include "window.hpp"
#include "window_system.hpp"


namespace hpr::gpu::glfw
{

WindowSystem::WindowSystem() :
    gpu::WindowSystem {Provider::GLFW}
{
    if (!glfwInit())
        throw std::runtime_error("Cannot initialize GLFW");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

WindowSystem::~WindowSystem()
{
    for (auto& window : p_windows)
        window->close();
    glfwTerminate();
}

gpu::Window* WindowSystem::newWindow()
{
    p_windows.push(new glfw::Window);

    return static_cast<gpu::Window*>(p_windows.back());
}

std::function<GLFWglproc(const char*)> WindowSystem::deviceProcAddress() const
{
    return std::function<GLFWglproc(const char*)>(glfwGetProcAddress);
}

}