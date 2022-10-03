#pragma once

#include "../texture.hpp"


namespace hpr::gpu::opengl
{

class Texture : public gpu::Texture
{
    friend class Device;

protected:

    unsigned int p_textureIndex;

public:

    Texture();

    ~Texture();

};

}