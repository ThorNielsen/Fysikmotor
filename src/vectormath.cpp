#include "include/vectormath.hpp"

#include "include/debug.hpp"

#include "lib/glm/glm/gtx/orthonormalize.hpp"

vec3 closest_point_between(line n, line m)
{
    vec3 a = n.p;
    vec3 b = n.r;
    vec3 c = m.p;
    vec3 d = m.r;
    float bb = dot(b,b);
    float bd = dot(b,d);
    float snum = dot(a,d)*bb+bd*dot(b,c)-bd*dot(a,b)-bb*dot(c,d);
    float sden = bb*dot(d,d)-bd*bd;
    float s = snum / sden;
    float t = (dot(b,c) + bd*s-dot(a,b)) / bb;
    return (a+b*t+c+d*s)/2.0f;
}

void line_line(line n, line m, float& t, float& s)
{
    vec3 a = n.p;
    vec3 b = n.r;
    vec3 c = m.p;
    vec3 d = m.r;
    float bb = dot(b,b);
    float bd = dot(b,d);
    float snum = dot(a,d)*bb+bd*dot(b,c)-bd*dot(a,b)-bb*dot(c,d);
    float sden = bb*dot(d,d)-bd*bd;
    s = snum / sden;
    t = (dot(b,c) + bd*s-dot(a,b)) / bb;
}

void orthonormalise(mat3& m)
{
    m = glm::orthonormalize(m);
}
