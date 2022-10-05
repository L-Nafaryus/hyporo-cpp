#pragma once

#include "context.hpp"

#include <string>


namespace hpr::gpu
{

class Shader : public Context
{
    friend class Device;

public:

    enum class ShaderType
    {
        Vertex,
        Geometry,
        Fragment,
        ShaderTypeCount
    };

protected:

    std::string p_filename;
    std::string p_label;
    ShaderType p_type;

public:

    // Constructors

    Shader();

    Shader(DeviceAPI api);

    virtual ~Shader();

    // Member functions

    const std::string filename() const;

    const std::string label() const;

    const ShaderType type() const;
};

} // end namespace hpr::gpu