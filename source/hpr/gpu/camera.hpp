#pragma once

#include "../math.hpp"


namespace hpr::gpu
{

class Camera
{

protected:

    vec3 p_front;
    vec3 p_up;
    vec3 p_left;

    scalar p_yaw;
    scalar p_pitch;
    scalar p_roll;

    vec3 p_position;
    vec3 p_target;

    scalar p_distance;

public:

    Camera() :
        p_front {0., 0., -1.},
        p_up {0., 0., 1.},
        p_left {1., 0., 0.}
    {}

    virtual
    ~Camera() = default;

};

}