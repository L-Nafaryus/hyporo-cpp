#pragma once

#include "context.hpp"
#include "shader.hpp"

#include "../hyplib/array/array.hpp"


namespace hpr::gpu
{

class ShaderProgram : public Context
{
    friend class Device;

protected:

    sarray<Shader*, (size_t)Shader::ShaderType::ShaderTypeCount> p_slots;
    bool p_isLinked;

public:

    // Constructors

    ShaderProgram();

    ShaderProgram(DeviceAPI api);

    virtual ~ShaderProgram();

    // Member functions

    const Shader* getShader(Shader::ShaderType type) const;
};

}