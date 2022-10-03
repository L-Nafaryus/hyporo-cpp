
#include "device.hpp"
#include "opengl/device.hpp"


namespace hpr::gpu
{

    Device::Device() :
        Context {DeviceAPI::Unknown}
    {}

    Device::Device(DeviceAPI api) :
        Context {api}
    {}

    Device::~Device()
    {}

    void Device::create(Device **device, DeviceAPI api)
    {
        if (device == nullptr)
            throw "Invalid parameter";
        if (api == DeviceAPI::Unknown)
            throw "Invalid parameter";

        *device = nullptr;

        if (api == DeviceAPI::OpenGL)
            *device = new opengl::Device;
        else
            throw "Unsupported device";
    }
}