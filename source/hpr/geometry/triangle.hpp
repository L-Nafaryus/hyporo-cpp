#include "../math.hpp"

namespace hpr::geometry
{

vec2 circumCentre(vec2 p1, vec2 p2, vec2 p3)
{
    vec2 pb1 {(p1 + p2) * 0.5};
    vec2 pb2 {(p2 + p3) * 0.5};
    scalar s1 = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    scalar s2 = (p3[1] - p2[1]) / (p3[0] - p2[0]);
    return vec2(pb1[0] + s1 * pb1[1], pb2[0] + s2 * pb2[1]) * mat2(1, s1, 1, s2).inv();
}

}