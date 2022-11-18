#include "render_target.hpp"


namespace hpr::gpu
{

RenderTarget::RenderTarget() :
    Context {DeviceAPI::Unknown},
    p_posX {},
    p_posY {},
    p_width {},
    p_height {},
    p_type {Type::Unknown},
    p_hasColorData {true},
    p_hasDepthBuffer {false},
    p_depthTestEnabled {false},
    p_parent {nullptr}
{}

RenderTarget::RenderTarget(DeviceAPI api) :
    Context {api},
    p_posX {},
    p_posY {},
    p_width {},
    p_height {},
    p_type {Type::Unknown},
    p_hasColorData {true},
    p_hasDepthBuffer {false},
    p_depthTestEnabled {false},
    p_parent {nullptr}
{}

RenderTarget::~RenderTarget() = default;

}
