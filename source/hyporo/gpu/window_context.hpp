#pragma once


namespace hpr::gpu
{

class WindowContext
{

public:

    enum class Provider
    {
        Unknown,
        GLFW
    };

private:

    Provider p_provider;

public:

    inline
    WindowContext() :
        p_provider {Provider::Unknown}
    {}

    inline
    WindowContext(Provider provider) :
        p_provider {provider}
    {}

    virtual
    ~WindowContext() = default;

    bool checkCompability(const WindowContext* ctx) const
    {
        return (ctx != nullptr) ? ctx->p_provider == p_provider : true;
    }

};

}
