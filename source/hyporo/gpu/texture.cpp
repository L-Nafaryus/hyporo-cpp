
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

Texture::~Texture()
{}

}
