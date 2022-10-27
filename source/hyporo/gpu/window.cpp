#include "window.hpp"


namespace hpr::gpu
{

Window::Window() :
    WindowContext(Provider::Unknown),
    p_width {0},
    p_height {0},
    p_posX {0},
    p_posY {0},
    p_title {},
    p_state {State::Hidden},
    p_style {Style::Unknown},
    p_parent {nullptr},
    p_monitor {nullptr},
    p_isActive {true},
    p_isResizing {false}
{}

Window::~Window() = default;

void Window::init(const std::string& title, Style style, int x, int y, int width, int height, Window* parent, Monitor* monitor)
{
    p_width = width;
    p_height = height;
    p_posX = x;
    p_posY = y;

    p_title = title;
    p_state = State::Hidden;
    p_style = style;

    p_parent = parent;
    p_monitor = monitor;
}

void Window::init(const std::string& title, Style style, Window* parent, Monitor* monitor)
{
    init(title, style, monitor->originX(), monitor->originY(), monitor->width(), monitor->height(), parent, monitor);
}

void Window::state(State state)
{
    p_state = state;
}

void Window::close()
{
    state(State::Closed);
}

void Window::restore()
{
    State prevState {p_state};
    init(p_title, p_style, p_posX, p_posY, p_width, p_height, p_parent, p_monitor);
    state(prevState);
}

bool Window::isOpen() const
{
    return p_state != State::Closed;
}

bool Window::isActive() const
{
    return p_isActive;
}

void Window::size(int width, int height)
{
    resizeCallback(width, height);
}

void Window::position(int x, int y)
{
    moveCallback(x, y);
}

void Window::title(const std::string& title)
{
    p_title = title;
}

void Window::style(Style style)
{
    p_style = style;
}

void Window::resizeCallback(int width, int height)
{
    p_width = width;
    p_height = height;
}

void Window::moveCallback(int x, int y)
{
    p_posX = x;
    p_posY = y;
}

}
