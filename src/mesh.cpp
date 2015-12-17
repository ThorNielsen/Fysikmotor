#include "mesh.hpp"
#include <iostream>

mesh::mesh()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(2, m_buffers);
    m_valid = true;
}

mesh::mesh(const mesh& other)
{
    m_vertices = other.m_vertices;
    m_indices = other.m_indices;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(2, m_buffers);
    m_valid = true;
}

mesh::mesh(mesh&& o)
{
    m_buffers[0] = o.m_buffers[0];
    m_buffers[1] = o.m_buffers[2];
    m_vao = o.m_vao;
    m_vertices = std::move(o.m_vertices);
    m_indices = std::move(o.m_indices);
    m_valid = true;
    o.m_valid = false;
}

mesh& mesh::operator=(mesh&& o)
{
    m_buffers[0] = o.m_buffers[0];
    m_buffers[1] = o.m_buffers[2];
    m_vao = o.m_vao;
    m_vertices = std::move(o.m_vertices);
    m_indices = std::move(o.m_indices);
    m_valid = true;
    o.m_valid = false;
    return *this;
}

mesh::~mesh()
{
    if (!m_valid) return;
    glDeleteBuffers(2, m_buffers);
    glDeleteVertexArrays(1, &m_vao);
}

void mesh::add_flat_cube(vec3 radii)
{
    size_t start = m_vertices.size();
    m_vertices.push_back({{radii.x, radii.y, radii.z},{-1,0,0}}); // 0
    m_vertices.push_back({{radii.x, radii.y, -radii.z},{-1,0,0}}); // 1
    m_vertices.push_back({{radii.x, -radii.y, radii.z},{-1,0,0}}); // 2
    m_vertices.push_back({{radii.x, -radii.y, -radii.z},{-1,0,0}}); // 3
    m_vertices.push_back({{-radii.x, radii.y, radii.z},{1,0,0}}); // 4
    m_vertices.push_back({{-radii.x, radii.y, -radii.z},{0,0,1}}); // 5
    m_vertices.push_back({{-radii.x, -radii.y, radii.z},{1,0,0}}); // 6
    m_vertices.push_back({{-radii.x, -radii.y, -radii.z},{0,0,1}}); // 7

    m_vertices.push_back({{radii.x, radii.y, radii.z},{0,0,-1}}); // 8
    m_vertices.push_back({{radii.x, radii.y, -radii.z},{0,0,1}}); // 9
    m_vertices.push_back({{radii.x, -radii.y, radii.z},{0,0,-1}}); // 10
    m_vertices.push_back({{radii.x, -radii.y, -radii.z},{0,0,1}}); // 11
    m_vertices.push_back({{-radii.x, radii.y, radii.z},{0,0,-1}}); // 12
    m_vertices.push_back({{-radii.x, radii.y, -radii.z},{1,0,0}}); // 13
    m_vertices.push_back({{-radii.x, -radii.y, radii.z},{0,0,-1}}); // 14
    m_vertices.push_back({{-radii.x, -radii.y, -radii.z},{1,0,0}}); // 15

    m_vertices.push_back({{radii.x, radii.y, radii.z},{0,1,0}}); // 16
    m_vertices.push_back({{radii.x, radii.y, -radii.z},{0,1,0}}); // 17
    m_vertices.push_back({{radii.x, -radii.y, radii.z},{0,-1,0}}); // 18
    m_vertices.push_back({{radii.x, -radii.y, -radii.z},{0,-1,0}}); // 19
    m_vertices.push_back({{-radii.x, radii.y, radii.z},{0,1,0}}); // 20
    m_vertices.push_back({{-radii.x, radii.y, -radii.z},{0,1,0}}); // 21
    m_vertices.push_back({{-radii.x, -radii.y, radii.z},{0,-1,0}}); // 22
    m_vertices.push_back({{-radii.x, -radii.y, -radii.z},{0,-1,0}}); // 23

    // {-1,0,0}
    m_indices.push_back(0+start);    m_indices.push_back(2+start);
    m_indices.push_back(3+start);    m_indices.push_back(0+start);
    m_indices.push_back(3+start);    m_indices.push_back(1+start);

    // {0,0,1}
    m_indices.push_back(9+start);    m_indices.push_back(11+start);
    m_indices.push_back(7+start);    m_indices.push_back(9+start);
    m_indices.push_back(7+start);    m_indices.push_back(5+start);

    // {1,0,0}
    m_indices.push_back(13+start);    m_indices.push_back(15+start);
    m_indices.push_back(6+start);    m_indices.push_back(13+start);
    m_indices.push_back(6+start);    m_indices.push_back(4+start);

    // {0,0,-1}
    m_indices.push_back(12+start);    m_indices.push_back(14+start);
    m_indices.push_back(10+start);    m_indices.push_back(12+start);
    m_indices.push_back(10+start);    m_indices.push_back(8+start);

    // {0,-1,0}
    m_indices.push_back(18+start);    m_indices.push_back(22+start);
    m_indices.push_back(23+start);    m_indices.push_back(18+start);
    m_indices.push_back(23+start);    m_indices.push_back(19+start);

    // {0,1,0}
    m_indices.push_back(20+start);    m_indices.push_back(16+start);
    m_indices.push_back(17+start);    m_indices.push_back(20+start);
    m_indices.push_back(17+start);    m_indices.push_back(21+start);
}

void mesh::add_cube(vec3 radii)
{
    size_t start = m_vertices.size();
    m_vertices.push_back({{radii.x, radii.y, radii.z},{0,0,0}});
    m_vertices.push_back({{radii.x, radii.y, -radii.z},{0,0,1}});
    m_vertices.push_back({{radii.x, -radii.y, radii.z},{0,1,0}});
    m_vertices.push_back({{radii.x, -radii.y, -radii.z},{0,1,1}});
    m_vertices.push_back({{-radii.x, radii.y, radii.z},{1,0,0}});
    m_vertices.push_back({{-radii.x, radii.y, -radii.z},{1,0,1}});
    m_vertices.push_back({{-radii.x, -radii.y, radii.z},{1,1,0}});
    m_vertices.push_back({{-radii.x, -radii.y, -radii.z},{1,1,1}});

    m_indices.push_back(0+start);    m_indices.push_back(2+start);
    m_indices.push_back(3+start);    m_indices.push_back(0+start);
    m_indices.push_back(3+start);    m_indices.push_back(1+start);

    m_indices.push_back(1+start);    m_indices.push_back(3+start);
    m_indices.push_back(7+start);    m_indices.push_back(1+start);
    m_indices.push_back(7+start);    m_indices.push_back(5+start);

    m_indices.push_back(5+start);    m_indices.push_back(7+start);
    m_indices.push_back(6+start);    m_indices.push_back(5+start);
    m_indices.push_back(6+start);    m_indices.push_back(4+start);

    m_indices.push_back(4+start);    m_indices.push_back(6+start);
    m_indices.push_back(2+start);    m_indices.push_back(4+start);
    m_indices.push_back(2+start);    m_indices.push_back(0+start);

    m_indices.push_back(2+start);    m_indices.push_back(6+start);
    m_indices.push_back(7+start);    m_indices.push_back(2+start);
    m_indices.push_back(7+start);    m_indices.push_back(3+start);

    m_indices.push_back(4+start);    m_indices.push_back(0+start);
    m_indices.push_back(1+start);    m_indices.push_back(4+start);
    m_indices.push_back(1+start);    m_indices.push_back(5+start);

}

void mesh::update()
{
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vertex),
                 &m_vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint),
                 &m_indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertex), (void*)(sizeof(vec3)));

#ifndef NDEBUG
    glBindVertexArray(0);
#endif // NDEBUG
}

void mesh::render()
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}
