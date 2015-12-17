#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED
#include <vector>
#include "include/opengl.hpp"
#include "include/vectormath.hpp"

struct vertex
{
    vec3 position;
    vec3 normal;
};

class mesh
{
public:
    mesh();
    mesh(const mesh&);
    mesh(mesh&&);
    mesh& operator=(mesh&&);
    ~mesh();
    void add_cube(vec3 radii); // Avg. runtime: 1.5-1.6 microseconds
    void add_flat_cube(vec3 radii); // Avg. runtime: 2.7-2.8 microseconds
    void add(vertex t)
    {
        m_vertices.push_back(t);
        m_indices.push_back(m_indices.size());
    }
    void update();
    void render();
    inline const std::vector<vertex>& verts()
    {
        return m_vertices;
    }
    inline const std::vector<GLuint> indices()
    {
        return m_indices;
    }
private:
    bool m_valid;
    GLuint m_buffers[2];
    GLuint m_vao;
    std::vector<vertex> m_vertices{};
    std::vector<GLuint> m_indices{};
};

#endif // MESH_HPP_INCLUDED
