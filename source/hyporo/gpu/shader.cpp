
#include "shader.hpp"


namespace hpr::gpu
{

Shader::Shader() :
    Context {DeviceAPI::Unknown},
    p_filename {"\0"},
    p_label {"\0"},
    p_type {ShaderType::Vertex}
{}

Shader::Shader(DeviceAPI api) :
    Context {api},
    p_filename {"\0"},
    p_label {"\0"},
    p_type {ShaderType::Vertex}
{}

Shader::~Shader() 
{}

const std::string Shader::filename() const
{
    return p_filename;
}

const std::string Shader::label() const
{
    return p_label;
}

const Shader::ShaderType Shader::type() const
{
    return p_type;
}

}