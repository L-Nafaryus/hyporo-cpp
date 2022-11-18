
#include "texture.hpp"


namespace hpr::gpu
{

Texture::Texture() :
    Context {DeviceAPI::Unknown},
    p_filename {},
    p_width {0},
    p_height {0}
{}


Texture::Texture(DeviceAPI api) :
        Context {api},
        p_filename {},
        p_width {0},
        p_height {0}
{}

Texture::~Texture() = default;

std::string Texture::filename() const
{
    return p_filename;
}

int Texture::width() const
{
    return p_width;
}

int Texture::height() const
{
    return p_height;
}

}
