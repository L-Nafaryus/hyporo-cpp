
#include "device.hpp"
#include "buffer.hpp
#include "shader.hpp"
#include "shader_program.hpp"
#include "texture.hpp"


namespace hpr::gpu::opengl
{

Device::Device() :
    gpu::Device {DeviceAPI::OpenGL},
    p_isInitialized {false}
{}

Device::~Device()
{}


}