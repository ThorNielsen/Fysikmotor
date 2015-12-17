#include "shader.hpp"
#include <iostream>
#include <fstream>

shader::~shader()
{
    for (GLuint shader : m_shaders)
    {
        glDeleteShader(shader);
    }
    if (m_linked)
    {
        glDeleteProgram(m_program);
    }
}

void shader::attachf(GLenum type, std::string path)
{
    std::string shader_code;
    std::ifstream file(path.c_str(), std::ios::in);
    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        shader_code.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&shader_code[0], shader_code.size());
        file.close();
    }


    GLuint shader_id = glCreateShader(type);

    const char* shader_code_ptr = shader_code.c_str();
    glShaderSource(shader_id, 1, &shader_code_ptr, NULL);
    glCompileShader(shader_id);

    m_shaders.push_back(shader_id);

    // glGetShaderiv(.., GL_COMPILE_STATUS, ..) sometimes gives wrong results
    // as it returns GL_TRUE even though compilation failed.
    // Checking the log length is more reliable although warnings probably
    // will be treated as errors.
    GLint log_length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 1)
    {
        std::string msg;
        msg.resize(log_length);
        glGetShaderInfoLog(shader_id, log_length, NULL, &msg[0]);
        std::cerr << "E:Compiling: " << msg << '\n';
    }
}

void shader::link()
{
    if (m_linked)
    {
        glDeleteProgram(m_program);
    }
    else m_linked = true;
    m_program = glCreateProgram();

    for (size_t i = 0; i < m_shaders.size(); ++i)
    {
        glAttachShader(m_program, m_shaders[i]);
    }
    glLinkProgram(m_program);

    // Same as in attachf(...), log length is needed to determine result.
    GLint log_length;
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 1)
    {
        std::string msg;
        msg.resize(log_length);
        glGetShaderInfoLog(m_program, log_length, NULL, &msg[0]);
        std::cerr << "E:Linking: " << msg << '\n';
    }

    // Misc cleanup
    for (size_t i = 0; i < m_shaders.size(); ++i)
    {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    m_shaders.clear();
}
