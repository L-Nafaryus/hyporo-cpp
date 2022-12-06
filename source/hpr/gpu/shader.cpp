
#include "shader.hpp"


namespace hpr::gpu
{

Shader::Shader() :
    Context {DeviceAPI::Unknown},
    p_filename {},
    p_label {},
    p_type {ShaderType::Vertex}
{}

Shader::Shader(DeviceAPI api) :
    Context {api},
    p_filename {},
    p_label {},
    p_type {ShaderType::Vertex}
{}

Shader::~Shader() = default;

std::string Shader::filename() const
{
    return p_filename;
}

std::string Shader::label() const
{
    return p_label;
}

Shader::ShaderType Shader::type() const
{
    return p_type;
}

}