
#include "device.hpp"
#include "opengl/device.hpp"


namespace hpr::gpu
{

Device::Device() :
    Context {DeviceAPI::Unknown},
    p_currentVertexBuffer {},
    p_currentIndexBuffer {},
    p_currentShaderProgram {}
{}

Device::Device(DeviceAPI api) :
    Context {api},
    p_currentVertexBuffer {},
    p_currentIndexBuffer {},
    p_currentShaderProgram {}
{}

Device::~Device()
{}

// Global functions

void Device::create(Device** device, DeviceAPI api)
{
    if (device == nullptr)
        throw std::invalid_argument("Invalid parameter 'nullptr'");
    if (api == DeviceAPI::Unknown)
        throw std::invalid_argument("Cannot create device for 'Unknown'");

    *device = nullptr;

    if (api == DeviceAPI::OpenGL)
        *device = new opengl::Device;
    else
        throw std::invalid_argument("Unsupported device");
}

// Buffers

void Device::useVertexBuffer(Buffer* buffer, int stride, int offset)
{
    if (buffer)
    {
        if (buffer->p_type == Buffer::BufferType::Vertex)
        {
            p_currentVertexBuffer = buffer;
            p_currentVertexBuffer->p_stride = stride;
        }
        else
            throw "Incompatible buffer";
    }
    else
        p_currentVertexBuffer = nullptr;
}

void Device::useIndexBuffer(Buffer* buffer, int offset)
{
    if (buffer)
    {
        if (buffer->p_type == Buffer::BufferType::Index)
        {
            p_currentIndexBuffer = buffer;
        }
        else
            throw "Incompatible buffer";
    }
    else
        p_currentIndexBuffer = nullptr;
}

void Device::destroyBuffer(Buffer*& buffer)
{
    if (!buffer)
        throw "Invalid parameter";

    for (auto iter = p_buffers.begin(); iter != p_buffers.end(); ++iter)
        if (&*iter == &*buffer)
            p_buffers.erase(iter);
    buffer = nullptr;
}

// Shaders

void Device::destroyShader(Shader *&shader)
{
    if (shader == nullptr)
        throw "Invalid parameter";

    for (auto iter = p_shaders.begin(); iter != p_shaders.end(); ++iter)
        if (&*iter == &*shader)
            p_shaders.erase(iter);
    shader = nullptr;
}

// Shader programs

void Device::attachShader(ShaderProgram *program, Shader *shader)
{
    if (program == nullptr || shader == nullptr)
        throw "Invalid parameter";
    if (program->p_isLinked)
        throw "Shader program already linked";

    program->p_slots[(int)shader->p_type] = shader;
}

void Device::linkProgram(ShaderProgram *program)
{
    if (program == nullptr)
        throw "Invalid parameter";
    if (program->p_isLinked)
        throw "Shader program already linked";

    program->p_isLinked = true;
}

void Device::useShaderProgram(ShaderProgram *program)
{
    if (program != nullptr)
        if (!program->p_isLinked)
            throw "Shader program is not linked";

    p_currentShaderProgram = program;
}

void Device::destroyShaderProgram(ShaderProgram *&program, bool withShaders)
{
    if (program == p_currentShaderProgram)
        useShaderProgram(nullptr);

    if (withShaders)
        for (size_t n = 0; n < (size_t)Shader::ShaderType::ShaderTypeCount; n++)
            destroyShader(program->p_slots[n]);
    program = nullptr;
}

// Textures

void Device::destroyTexture(Texture *&texture)
{
    for (auto iter = p_textures.begin(); iter != p_textures.end(); ++iter)
        if (&*iter == &*texture)
            p_textures.erase(iter);
    texture = nullptr;
}

}