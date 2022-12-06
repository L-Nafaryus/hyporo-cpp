#pragma once


namespace hpr::gpu
{

class Context
{

public:

    enum class DeviceAPI
    {
        Unknown,
        OpenGL,
        DeviceAPICount
    };

private:

    DeviceAPI p_api;

public:

    // Constructors

    Context();

    explicit
    Context(DeviceAPI api);

    virtual
    ~Context();

    // Member functions

    bool checkCompability(const Context* ctx) const;
};

}