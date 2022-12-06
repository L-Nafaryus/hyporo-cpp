
#include "texture.hpp"


namespace hpr::gpu::opengl
{

Texture::Texture() :
    gpu::Texture {DeviceAPI::OpenGL},
    p_textureIndex {0}
{}

Texture::~Texture() = default;

}