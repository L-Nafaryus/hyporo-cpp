
#include "device.hpp"
#include "opengl/device.hpp"


namespace hpr::gpu
{

Device::Device() :
    Context {DeviceAPI::Unknown},
    p_currentVertexBuffer {},
    p_currentIndexBuffer {},
    p_currentUniformBuffer {},
    p_currentShaderProgram {}
{}

Device::Device(DeviceAPI api) :
    Context {api},
    p_currentVertexBuffer {},
    p_currentIndexBuffer {},
    p_currentUniformBuffer {},
    p_currentShaderProgram {}
{}

Device::~Device()
{
    for (auto& shader : p_shaders)
        delete shader;
    for (auto& buffer : p_buffers)
        delete buffer;
    for (auto& shaderProgram : p_shaderPrograms)
        delete shaderProgram;
    for (auto& texture : p_textures)
        delete texture;
}

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
            throw std::runtime_error("Incompatible buffer");
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
            throw std::runtime_error("Incompatible buffer");
    }
    else
        p_currentIndexBuffer = nullptr;
}

void Device::useUniformBuffer(Buffer* buffer, int slot)
{
    if (buffer)
    {
        if (buffer->p_type == Buffer::BufferType::Uniform)
            p_currentUniformBuffer = buffer;
        else
            throw std::runtime_error("Incompatible buffer");
    }
    else
        p_currentUniformBuffer = nullptr;
}

void Device::destroyBuffer(Buffer*& buffer)
{
    if (!buffer)
        throw std::runtime_error("Invalid parameter");

    for (auto iter = p_buffers.begin(); iter != p_buffers.end(); ++iter)
        if (*iter == buffer)
        {
            delete buffer;
            buffer = nullptr;
            p_buffers.remove(iter);
            break;
        }
}

Buffer* Device::activeBuffer(Buffer::BufferType type)
{
    switch (type)
    {
        case Buffer::BufferType::Vertex:
            return p_currentVertexBuffer;
        case Buffer::BufferType::Index:
            return p_currentIndexBuffer;
        case Buffer::BufferType::Uniform:
            return p_currentUniformBuffer;
        default:
            return nullptr;
    }
}

// Shaders

void Device::destroyShader(Shader* shader)
{
    if (shader == nullptr)
        throw std::runtime_error("Invalid parameter");

    for (auto iter = p_shaders.begin(); iter != p_shaders.end(); ++iter)
        if (*iter == shader)
        {
            delete shader;
            shader = nullptr;
            p_shaders.remove(iter);
            break;
        }
}

// Shader programs

void Device::attachShader(ShaderProgram *program, Shader *shader)
{
    if (program == nullptr || shader == nullptr)
        throw std::runtime_error("Invalid parameter");
    if (program->p_isLinked)
        throw std::runtime_error("Shader program already linked");

    program->p_slots[(int)shader->p_type] = shader;
}

void Device::linkProgram(ShaderProgram *program)
{
    if (program == nullptr)
        throw std::runtime_error("Invalid parameter");
    if (program->p_isLinked)
        throw std::runtime_error("Shader program already linked");

    program->p_isLinked = true;
}

void Device::useShaderProgram(ShaderProgram *program)
{
    if (program != nullptr)
        if (!program->p_isLinked)
            throw std::runtime_error("Shader program is not linked");

    p_currentShaderProgram = program;
}

void Device::destroyShaderProgram(ShaderProgram *&program, bool withShaders)
{
    if (program == p_currentShaderProgram)
        useShaderProgram(nullptr);

    if (withShaders)
        for (size_t n = 0; n < (size_t)Shader::ShaderType::ShaderTypeCount; n++)
            destroyShader(program->p_slots[n]);

    for (auto iter = p_shaderPrograms.begin(); iter != p_shaderPrograms.end(); ++iter)
        if (*iter == program)
        {
            delete program;
            program = nullptr;
            p_shaderPrograms.remove(iter);
            break;
        }
}

// Textures

void Device::destroyTexture(Texture *&texture)
{
    for (auto iter = p_textures.begin(); iter != p_textures.end(); ++iter)
        if (*iter == texture)
        {
            delete texture;
            texture = nullptr;
            p_textures.remove(iter);
            break;
        }
}

}