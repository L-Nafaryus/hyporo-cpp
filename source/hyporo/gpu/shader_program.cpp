
#include "shader_program.hpp"


namespace hpr::gpu
{

ShaderProgram::ShaderProgram() : 
    Context {DeviceAPI::Unknown},
    p_isLinked {false}
{}

ShaderProgram::ShaderProgram(DeviceAPI api) : 
    Context {api},
    p_isLinked {false}
{}

ShaderProgram::~ShaderProgram()
{}

}