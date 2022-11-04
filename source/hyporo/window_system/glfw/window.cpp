#include "window.hpp"
#include "monitor.hpp"

#include <stdexcept>


namespace hpr::gpu::glfw
{

Window::Window() :
    gpu::Window {Provider::GLFW},
    p_instance {nullptr}
{}

Window::~Window() = default;

void Window::init(const std::string& title, Style style, int x, int y, int width, int height, gpu::Window* parent, gpu::Monitor* monitor)
{
    if (!checkCompability(parent))
        throw std::invalid_argument("Incompatible window provider passed");

    gpu::Window::init(title, style, x, y, width, height, parent, monitor);

    glfw::Window* parent_ = dynamic_cast<glfw::Window*>(parent);
    glfw::Monitor* monitor_ = dynamic_cast<glfw::Monitor*>(monitor);

    p_instance = glfwCreateWindow(width, height, title.c_str(),
        monitor_ != nullptr ? monitor_->p_instance : nullptr, parent_ != nullptr ? parent_->p_instance : nullptr);
    if (p_instance == nullptr)
        throw std::runtime_error("Cannot create GLFW window");
    glfwMakeContextCurrent(p_instance);
    glfwSetWindowPos(p_instance, x, y);
    this->style(style);
}

void Window::init(const std::string& title, Style style, gpu::Window* parent, gpu::Monitor* monitor)
{
    init(title, style, monitor->originX(), monitor->originY(), monitor->width(), monitor->height(), parent, monitor);
}

void Window::state(State state)
{
    gpu::Window::state(state);
    switch (state)
    {
        case State::Visible:
            glfwShowWindow(p_instance);
            break;
        case State::Hidden:
            glfwHideWindow(p_instance);
            break;
        case State::Maximized:
            glfwMaximizeWindow(p_instance);
            break;
        case State::Minimized:
            glfwIconifyWindow(p_instance);
            break;
        case State::Closed:
            glfwSetWindowShouldClose(p_instance, GLFW_TRUE);
            break;
        default:
            break;
    }
}

void Window::close()
{
    gpu::Window::close();
    glfwDestroyWindow(p_instance);
    p_instance = nullptr;
}

void Window::style(Style style)
{
    gpu::Window::style(style);
    if (style == Style::Windowed)
    {
        glfwSetWindowMonitor(p_instance, nullptr, p_posX, p_posY, p_width, p_height, GLFW_DONT_CARE);
    }
    else if (style == Style::Fullscreen)
    {
        glfwSetWindowMonitor(p_instance, glfwGetPrimaryMonitor(), p_posX, p_posY, p_width, p_height, GLFW_DONT_CARE);
    }
    else if (style == Style::Popup)
    {
        //throw std::runtime_error("Popup style is not supported");
    }
}

void Window::swapBuffers()
{
    glfwSwapBuffers(p_instance);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

}