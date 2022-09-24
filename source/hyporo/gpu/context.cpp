
#include "context.hpp"


namespace hpr::gpu
{

Context::Context() :
    p_api {DeviceAPI::Unknown}
{}

Context::Context(DeviceAPI api) :
    p_api {api}
{}

Context::~Context() 
{}

bool Context::checkCompability(const Context* ctx) const
{
    return (ctx != nullptr) ? ctx->p_api == p_api : true;
}

}