#ifndef RIGID_BODY_HPP_INCLUDED
#define RIGID_BODY_HPP_INCLUDED
#include "include/opengl.hpp"
#include "include/mesh.hpp"
#include "include/window.hpp"
#include <vector>

struct collision_info
{
    vec3 point; // Point of collision (in a's reference frame)
    vec3 normal; // Collision normal
    size_t id1; // Id of object 1 (a)
    size_t id2; // Id of object 2 (b)
};

constexpr float IMMOBILE = 0.0f;

class rigid_body
{
public:
    rigid_body(vec3 radii, vec3 position, float mass)
    {
        m_radii = radii;
        m_mesh.add_flat_cube(radii);
        m_mesh.update();
        m_pos = position;
        if (mass == IMMOBILE)
        {
            m_mass = 0.0f;
            m_inv_mass = 0.0f;
        }
        else
        {
            m_mass = mass;
            m_inv_mass = 1.0f / mass;
        }
        m_inertia = mat3(0.0f);
        m_inertia[0][0] = 1.f/12.f * m_mass *
            (radii.y * radii.y + radii.z * radii.z);
        m_inertia[1][1] = 1.f/12.f * m_mass *
            (radii.x * radii.x + radii.z * radii.z);
        m_inertia[2][2] = 1.f/12.f * m_mass *
            (radii.y * radii.y + radii.x * radii.x);
        m_iinertia = mass == IMMOBILE ? m_inertia : inverse(m_inertia);
        optimise();
    }
    rigid_body(const rigid_body&) = delete;
    rigid_body(rigid_body&&);
    void translate(vec3 dxyz)
    {
        m_pos += dxyz;
    }
    void rotate(vec3 axis, float angle)
    {
        m_apos = mat3(glm::rotate(mat4(m_apos), angle, axis));
    }
    mat4 tmatrix()
    {
        return glm::translate(mat4(1.0f), m_pos) * mat4(m_apos);
    }
    void input(window& w);
    void update(float dt);
    bool intersect(rigid_body&);
    void render(GLint mwloc, GLint mcloc, mat4 wv)
    {
        mat4 t = tmatrix();
        glUniformMatrix4fv(mwloc, 1, GL_FALSE, &t[0][0]);
        t = wv * t;
        glUniformMatrix4fv(mcloc, 1, GL_FALSE, &t[0][0]);
        m_mesh.render();
    }
private:
    void add_vertex(vec3);
    void add_axe(vec3);
    void optimise();
    mesh m_mesh;
    std::vector<vec3> m_vertices;
    std::vector<vec3> m_axes;
    mat3 m_inertia;
    mat3 m_iinertia;
    mat3 m_wiinertia;
    mat3 m_apos; // Angular position = rotation
    vec3 m_avel;
    vec3 m_radii;
    vec3 m_pos; // position
    vec3 m_vel; // velocity
    vec3 m_force;
    vec3 m_mom;
    vec3 m_amom;
    size_t m_edge_begin;
    float m_mass;
    float m_inv_mass;
};

#endif // RIGID_BODY_HPP_INCLUDED
