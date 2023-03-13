#pragma once

#include <hpr/gpu/context.hpp>
#include <hpr/gpu/monitor.hpp>

#include <string>
#include <stdexcept>
#include <functional>

#include <GLFW/glfw3.h>


namespace hpr::gpu
{

class Window
{

public:

    enum State
    {
        Visible,
        Hidden,
        Maximized,
        Minimized,
        Closed,
    };

    enum Style
    {
        Windowed,
        Fullscreen,
        Popup
    };

protected:

    int p_posX;
    int p_posY;
    int p_width;
    int p_height;
    std::string p_title;

    State p_state;
    Style p_style;
    Window* p_parent;
    Monitor* p_monitor;

    bool p_isActive;
    bool p_isResizing;

    GLFWwindow* p_instance;
    Context p_context;

public:

    inline
    Window() :
        p_posX {0},
        p_posY {0},
        p_width {0},
        p_height {0},
        p_title {},
        p_state {State::Hidden},
        p_style {Style::Windowed},
        p_parent {nullptr},
        p_monitor {nullptr},
        p_isActive {true},
        p_isResizing {false},
        p_context {}
    {
        p_instance = glfwCreateWindow(p_width, p_height, p_title.c_str(), p_monitor != nullptr ? p_monitor->instance() : nullptr, p_parent != nullptr ? p_parent->p_instance : nullptr);
        if (p_instance == nullptr)
            throw std::runtime_error("Cannot create Window");

        glfwMakeContextCurrent(p_instance);
        p_context.link();

        glfwSetWindowUserPointer(p_instance, this);

        glfwSetWindowPos(p_instance, p_posX, p_posY);

        this->state(p_state);
        this->style(p_style);
    }

    inline
    Window(int x, int y, int width, int height, const std::string& title, Style style, Window* parent, Monitor* monitor) :
        p_posX {x},
        p_posY {y},
        p_width {width},
        p_height {height},
        p_title {title},
        p_state {State::Visible},
        p_style {style},
        p_parent {parent},
        p_monitor {monitor},
        p_isActive {true},
        p_isResizing {false},
        p_context {}
    {
        //glfwInit();

        p_instance = glfwCreateWindow(p_width, p_height, p_title.c_str(), p_monitor != nullptr ? p_monitor->instance() : nullptr, p_parent != nullptr ? p_parent->p_instance : nullptr);
        if (p_instance == nullptr)
            throw std::runtime_error("Cannot create window");

        glfwMakeContextCurrent(p_instance);
        p_context.link();

        glfwSetWindowUserPointer(p_instance, this);

        glfwSetWindowPos(p_instance, p_posX, p_posY);

        this->state(p_state);
        this->style(p_style);
    }

    virtual
    ~Window()
    {
        p_context.destroy();
    }

    // member functions

    inline
    GLFWwindow* instance() const
    {
        return p_instance;
    }

    inline
    Context& context()
    {
        return p_context;
    }

    inline
    void pos(int x, int y)
    {
        p_posX = x;
        p_posY = y;
    }

    inline
    int x() const
    {
        return p_posX;
    }

    inline
    int y() const
    {
        return p_posY;
    }

    inline
    void size(int width, int height)
    {
        p_width = width;
        p_height = height;
    }

    inline
    int width() const
    {
        return p_width;
    }

    inline
    int height() const
    {
        return p_height;
    }

    inline
    void title(const std::string& title)
    {
        p_title = title;
    }

    inline
    std::string title() const
    {
        return p_title;
    }

    inline
    void state(State state)
    {
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
        p_state = state;
    }

    inline
    State state() const
    {
        return p_state;
    }

    inline
    void style(Style style)
    {
        switch (style)
        {
            case Style::Windowed:
                glfwSetWindowMonitor(p_instance, nullptr, p_posX, p_posY, p_width, p_height, GLFW_DONT_CARE);
                break;
            case Style::Fullscreen:
                glfwSetWindowMonitor(p_instance, glfwGetPrimaryMonitor(), p_posX, p_posY, p_width, p_height, GLFW_DONT_CARE);
                break;
            default:
                break;
        }
        p_style = style;
    }

    inline
    Style style() const
    {
        return p_style;
    }

    inline
    void destroy()
    {
        glfwDestroyWindow(p_instance);
        p_instance = nullptr;
    }

    inline
    void swapBuffers()
    {
        glfwSwapBuffers(p_instance);
    }

    inline
    void pollEvents()
    {
        glfwPollEvents();
    }

protected:

    std::function<void(Window* window, int width, int height)> onFramebufferResize;
    std::function<void(Window* window, int key, int scancode, int action, int mods)> onKeyClick;

public:

    void framebufferResizeCallback(const std::function<void(Window* window, int width, int height)>& event)
    {
        onFramebufferResize = event;

        glfwSetFramebufferSizeCallback(p_instance, [](GLFWwindow* window, int width, int height)
        {
            auto p = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (p->onFramebufferResize)
                p->onFramebufferResize(p, width, height);
        });
    }

    void keyClickCallback(const std::function<void(Window* window, int key, int scancode, int action, int mods)>& event)
    {
        onKeyClick = event;

        glfwSetKeyCallback(p_instance, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            auto p = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (p->onKeyClick)
                p->onKeyClick(p, key, scancode, action, mods);
        });
    }
};

}