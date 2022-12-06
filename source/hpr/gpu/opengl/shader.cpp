//
// Created by L-Nafaryus on 10/3/2022.
//

#include "shader.hpp"


namespace hpr::gpu::opengl
{

Shader::Shader() :
    gpu::Shader(DeviceAPI::OpenGL),
    p_shaderIndex {0}
{}

Shader::~Shader() = default;

}