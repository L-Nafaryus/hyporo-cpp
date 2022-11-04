#include "monitor.hpp"


namespace hpr::gpu::glfw
{

Monitor::Monitor() :
    gpu::Monitor {Provider::GLFW},
    p_instance {nullptr}
{}

Monitor::~Monitor() = default;

}