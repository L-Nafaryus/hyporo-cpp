#pragma once

#include "window.hpp"
#include "monitor.hpp"
#include "../hyplib/array/array.hpp"

#include <string>


namespace hpr::gpu
{

class WindowSystem : public WindowContext
{

protected:

    darray<Window*> p_windows;
    darray<Monitor*> p_monitors;

protected:

    WindowSystem();

    WindowSystem(Provider provider);

    virtual
    ~WindowSystem();

public:

    // Global functions

    static
    WindowSystem* create(Provider provider);

    static
    void destroy(WindowSystem*& ws);

    // Window interface

    virtual
    Window* newWindow() = 0;

    Window* window(int index);

    void closeWindow(Window* window);

    void destroyWindow(Window* window);

    // Monitor interface

    //virtual
    //void newMonitor(Monitor** monitor) = 0;

    Monitor* monitor(int index);

    void destroyMonitor(Monitor* monitor);

};

}