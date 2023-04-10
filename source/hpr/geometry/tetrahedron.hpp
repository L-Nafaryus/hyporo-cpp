#pragma once

#include <hpr/math.hpp>

namespace hpr::geometry
{

vec3 circumCentre(vec3 p1, vec3 p2, vec3 p3, vec3 p4)
{
    vec3 e1 = p2 - p1;
    vec3 e2 = p3 - p1;
    vec3 e3 = p4 - p1;
    mat3 A;
    A.row(0, e1);
    A.row(1, e2);
    A.row(2, e3);
    vec3 B = 0.5 * vec3(sum(pow(p2, 2)) - sum(pow(p1, 2)), sum(pow(p3, 2)) - sum(pow(p1, 2)), sum(pow(p4, 2)) - sum(pow(p1, 2)));
    return A.inv() * B;
}

}