#pragma once

#include <hpr/math.hpp>
#include "camera.hpp"
#include "entity.hpp"

using namespace hpr;

vec3 unproject(const vec3& win, const mat4& model, const mat4& proj, const vec4& viewport)
{
    mat4 inverse = inv(proj * model);
    vec4 tmp = vec4(win, 1.);
    tmp[0] = (tmp[0] - viewport[0]) / viewport[2];
    tmp[1] = (tmp[1] - viewport[1]) / viewport[3];
    tmp = tmp * 2.f - 1.f;
    vec4 obj = inverse * tmp;
    obj /= obj[3];
    return vec3(obj);
}

class Ray
{
public:
    vec3 p_position;
    vec3 p_direction;


    Ray() :
        p_position {},
        p_direction {}
    {}

    Ray(const vec3& position, const vec3& direction) :
        p_position {position},
        p_direction {direction}
    {}

    void fromScreen(int x, int y, int width, int height, Camera* camera, Entity* entity)
    {
        vec4 start {2.f * (float)x / (float)width - 1.f, 2.f * (float)y / (float)height - 1.f, 0.f, 1.f};
        vec4 end {2.f * (float)x / (float)width - 1.f, 2.f * (float)y / (float)height - 1.f, 1.f, 1.f};

        mat4 invPV = inv(transpose(camera->projection()) * transpose(camera->view()));
        vec4 startWorld = invPV * start;
        startWorld /= startWorld[3];
        vec4 endWorld = invPV * end;
        endWorld /= endWorld[3];

        
        //vec3 end = unproject(vec3(x, y, 0), entity->model(), camera->projection(), vec4(0, 0, width, height));

        p_position = vec3(startWorld);
        p_direction = normalize(vec3(endWorld - startWorld));
    }

    bool obb(const vec3& aabbMin, const vec3& aabbMax, Entity* entity, float& tMin, float& tMax)
    {
        tMin = 0.;
        tMax = 1e+5;

        mat4 model = entity->model();
        vec3 obbPos {model(0, 3), model(1, 3), model(2, 3)};
        vec3 delta = obbPos - p_position;

        for (auto n = 0; n < 3; ++n)
        {
            vec3 axis {model(0, n), model(1, n), model(2, n)};
            scalar e = dot(axis, delta);
            scalar f = dot(p_direction, axis);

            if (hpr::abs(f) > 1e-3)
            {
                scalar t1 = (e + aabbMin[n]) / f;
                scalar t2 = (e + aabbMax[n]) / f;

                if (t1 > t2)
                    std::swap(t1, t2);
                if (t2 < tMax)
                    tMax = t2;
                if (t1 > tMin)
                    tMin = t1;
                if (tMax < tMin)
                    return false;
            }
            else
            {
                if (-e + aabbMin[n] > 0.f || -e + aabbMax[n] < 0.f)
                    return false;
            }
        }
        //dist = tMin;
        return true;
    }
};


/*
bool RayIntersectsTriangle(Vector3D rayOrigin,
                           Vector3D rayVector,
                           Triangle* inTriangle,
                           Vector3D& outIntersectionPoint)
{
    const float EPSILON = 0.0000001;
    Vector3D vertex0 = inTriangle->vertex0;
    Vector3D vertex1 = inTriangle->vertex1;
    Vector3D vertex2 = inTriangle->vertex2;
    Vector3D edge1, edge2, h, s, q;
    float a,f,u,v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = rayVector.crossProduct(edge2);
    a = edge1.dotProduct(h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    f = 1.0/a;
    s = rayOrigin - vertex0;
    u = f * s.dotProduct(h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = s.crossProduct(edge1);
    v = f * rayVector.dotProduct(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * edge2.dotProduct(q);
    if (t > EPSILON) // ray intersection
    {
        outIntersectionPoint = rayOrigin + rayVector * t;
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}
 */