#pragma once

#include "context.hpp"

#include <string>


namespace hpr::gpu
{

class Texture : public Context
{
protected:

    std::string p_filename;
    int p_width;
    int p_height;

public:

    // Constructors

    Texture();

    Texture(DeviceAPI api);

    virtual ~Texture();

    // Member functions

    const std::string filename() const();

    const int width() const;

    const int height() const;
};

} // end namespace hpr::gpu