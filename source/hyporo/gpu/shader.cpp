
#include "shader.hpp"


namespace hpr::gpu
{

Shader::Shader() :
    Context {DeviceAPI::Unknown},
    p_filename {},
    p_label {}
{}

Shader::Shader(DeviceAPI api) :
    Context {api},
    p_filename {},
    p_label {}
{}

Shader::~Shader() 
{}

}