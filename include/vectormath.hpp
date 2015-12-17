#ifndef VECTORMATH_HPP_INCLUDED
#define VECTORMATH_HPP_INCLUDED

#define GLM_FORCE_RADIANS
#include "lib/glm/glm/glm.hpp"
#include "lib/glm/glm/gtc/matrix_transform.hpp"

using vec4 = glm::vec4;
using vec3 = glm::vec3;
using vec2 = glm::vec2;

using dvec2 = glm::dvec2;

using ivec2 = glm::ivec2;

using mat4 = glm::mat4;
using mat3 = glm::mat3;
using mat2 = glm::mat2;
using glm::cross;
using glm::dot;

constexpr float CMP_EPSILON = 0.00001f;
inline bool flt_eq(float a, float b)
{
    return fabs(a-b) < CMP_EPSILON;
}

constexpr float CMP_EPS_SQR = CMP_EPSILON * CMP_EPSILON;
inline bool zero_vector(vec3 t)
{
    return dot(t, t) < CMP_EPS_SQR;
}

inline bool same_position(vec3 a, vec3 b)
{
    return (fabs(a.x - b.x) < CMP_EPSILON) &&
            (fabs(a.y - b.y) < CMP_EPSILON) &&
            (fabs(a.z - b.z) < CMP_EPSILON);
}

// Assumes 'axe' is normalised, returns length of 'v' projected on 'axe'.
inline float nprojectf(vec3 v, vec3 axe)
{
    return dot(v, axe);
}

// Returns length of 'v' projected on 'axe'.
inline float projectf(vec3 v, vec3 axe)
{
    return dot(v, axe) / dot(axe, axe);
}

// Line given by the parametric equation 'p+t*r'
struct line
{
    vec3 p;
    vec3 r;
};

vec3 closest_point_between(line, line);

void line_line(line, line, float& t, float& s);

void orthonormalise(mat3&);

inline mat3 skew(vec3 v)
{
    mat3 m(0.0f);
    m[0][1] = -v.z;
    m[0][2] = v.y;
    m[1][0] = v.z;
    m[1][2] = -v.x;
    m[2][0] = -v.y;
    m[2][1] = v.x;
    return m;
}

inline float length_sqr(vec3 v)
{
    return v.x*v.x+v.y*v.y+v.z*v.z;
}

inline float dist_sqr(vec3 a, vec3 b)
{
    return (a.x-b.x)*(a.x-b.x) +
            (a.y-b.y)*(a.y-b.y) +
            (a.z-b.z)*(a.z-b.z);
}

inline float cos_angle(vec3 a, vec3 b)
{
    return dot(a,b) / (length(a)*length(b));
}

#endif // VECTORMATH_HPP_INCLUDED
