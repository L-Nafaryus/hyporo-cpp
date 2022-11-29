#pragma once

#include "window_context.hpp"
#include "monitor.hpp"


namespace hpr::gpu
{

class Window : public WindowContext
{

public:

    enum class State
    {
        Unknown,
        Visible,
        Hidden,
        Maximized,
        Minimized,
        Closed,
    };

    enum class Style
    {
        Unknown,
        Windowed,
        Fullscreen,
        Popup
    };

protected:

    int p_width;
    int p_height;
    int p_posX;
    int p_posY;
    std::string p_title;

    State p_state;
    Style p_style;
    Window* p_parent;
    Monitor* p_monitor;

    bool p_isActive;
    bool p_isResizing;

public:

    Window();

    Window(Provider provider);

    virtual
    ~Window();

    // Member functions

    virtual
    void init(const std::string& title, Style style, int x, int y, int width, int height, Window* parent, Monitor* monitor);

    virtual
    void init(const std::string& title, Style style, Window* parent, Monitor* monitor);

    virtual
    void state(State state);

    virtual
    void close();

    virtual
    void restore();

    virtual
    bool isOpen() const;

    virtual
    bool isActive() const;

    virtual
    void size(int width, int height);

    virtual
    int width() const
    {
        return p_width;
    }

    virtual
    int height() const
    {
        return p_height;
    }

    virtual
    void position(int x, int y);

    virtual
    int posX() const
    {
        return p_posX;
    }

    virtual
    int posY() const
    {
        return p_posY;
    }

    virtual
    void title(const std::string& title);

    virtual
    void style(Style style);

    void resizeCallback(int width, int height);

    void moveCallback(int x, int y);

};

}