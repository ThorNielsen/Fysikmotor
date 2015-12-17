#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED
#include "include/opengl.hpp"
#include <string>
#include <vector>

class shader
{
public:
    shader() {};
    shader(const shader&) = delete;
    shader operator=(const shader&) = delete;
    inline void use() const { glUseProgram(m_program); }
    void attachf(GLenum type, std::string path);
    inline GLint uniform_location(const char* c)
    {
        return glGetUniformLocation(m_program, c);
    }
    void link();
    ~shader();
private:
    std::vector<GLuint> m_shaders{};
    GLuint m_program = 0;
    bool m_linked = false;
};

#endif // SHADER_HPP_INCLUDED
