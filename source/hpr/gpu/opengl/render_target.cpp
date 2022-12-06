#pragma once

#include "render_target.hpp"


namespace hpr::gpu::opengl
{

RenderTarget::RenderTarget() :
    gpu::RenderTarget {DeviceAPI::OpenGL}
{}

RenderTarget::~RenderTarget() = default;

}