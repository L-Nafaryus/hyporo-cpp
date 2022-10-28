
#include "shader_program.hpp"
#include "shader.hpp"


namespace hpr::gpu::opengl
{

ShaderProgram::ShaderProgram() :
    gpu::ShaderProgram {DeviceAPI::OpenGL},
    p_shaderProgramIndex {0}
{}

ShaderProgram::~ShaderProgram()
{}

Shader* ShaderProgram::shader(gpu::Shader::ShaderType type)
{
    return static_cast<opengl::Shader*>(p_slots[(size_t)type]);
}

}