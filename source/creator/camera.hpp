#pragma once

#include <hpr/math.hpp>

class Camera
{

public:

    enum Projection
    {
        Perspective,
        Orthographic
    };

protected:

    Projection p_projectionType;

    hpr::scalar p_distance;
    hpr::scalar p_aspect;
    hpr::scalar p_fieldOfView;
    hpr::scalar p_nearPlane;
    hpr::scalar p_farPlane;

    hpr::vec3 p_target;
    hpr::vec3 p_left;
    hpr::vec3 p_front;
    hpr::vec3 p_up;
    hpr::vec3 p_angles;

    hpr::vec3 p_position;

public:

    inline
    Camera() :
        p_projectionType {Perspective},
        p_distance {10},
        p_aspect {4.f / 3.f},
        p_fieldOfView {45},
        p_nearPlane {0.1},//{-100},
        p_farPlane {100},
        p_target {0, 0, 0},
        p_left {1, 0, 0},
        p_front {0, 0, -1},
        p_up {0, 0, 1},
        p_angles {45, 45, 0},
        p_position {}
    {}

    virtual
    ~Camera() = default;

    virtual inline
    void projection(Projection projection)
    {
        p_projectionType = projection;
    }

    hpr::scalar& aspect()
    {
        return p_aspect;
    }

    virtual inline
    hpr::mat4 projection() const
    {
        switch (p_projectionType)
        {
            case Perspective:
                return hpr::perspective(hpr::rad(p_fieldOfView), p_aspect, p_nearPlane, p_farPlane);
            case Orthographic:
                return hpr::ortho(-p_distance * p_aspect, p_distance * p_aspect, -p_distance, p_distance, p_nearPlane, p_farPlane);
        }
    }

    virtual inline
    hpr::mat4 view() = 0;

    virtual inline
    hpr::vec3& position()
    {
        return p_position;
    }
};

class OrbitCamera : public Camera
{
public:

    inline
    OrbitCamera() :
        Camera {}
    {}

    virtual
    ~OrbitCamera() = default;

    inline
    hpr::mat4 view() override
    {
        hpr::vec3 rotation {
                hpr::cos(hpr::rad(p_angles[1])) * hpr::sin(hpr::rad(p_angles[0])),
                hpr::cos(hpr::rad(p_angles[1])) * hpr::cos(hpr::rad(p_angles[0])),
                hpr::sin(hpr::rad(p_angles[1]))};
        hpr::vec3 pos = p_target + p_distance * rotation;

        p_front = p_target - pos;
        p_front /= hpr::length(p_front);

        p_up = hpr::vec3(0, 0, 1);
        p_left = hpr::normalize(hpr::cross(p_up, p_front));
        p_up = hpr::cross(p_front, p_left);

        p_position = pos;

        return hpr::lookAt(pos, pos + p_front, p_up);
    }

    void rotateEvent(float xdelta, float ydelta)
    {
        p_angles += hpr::vec3(xdelta, ydelta, 0);
    }

    void moveEvent(float xdelta, float ydelta)
    {
        p_target += (p_left * xdelta + p_up * ydelta) * 1e-3f;
    }

    void scrollEvent(float xdelta, float ydelta)
    {
        p_distance -= ydelta;
    }
};