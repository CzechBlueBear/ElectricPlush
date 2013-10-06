#include "shader_program.hpp"
#include "gl_error.hpp"

#include <stdexcept>
#include <iostream>

using namespace plush;

ShaderProgram::ShaderProgram()
    : m_id(0)
{
    m_id = glCreateProgram();
    GLError::check("ShaderProgram::ShaderProgram()");
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_id);
}

void ShaderProgram::attach(Shader &sh)
{
    glAttachShader(m_id, sh.m_id);
    GLError::check("ShaderProgram::attach()");
}

void ShaderProgram::link()
{
    glLinkProgram(m_id);
    GLError::check("ShaderProgram::link()");

    GLint success = 0;
    GLchar log[1024] = { 0 };

    // show link log if nonempty (it can contain warnings even if link succeeds)
    glGetProgramInfoLog(m_id, sizeof(log), NULL, log);
    if (log[0] != 0) {
        std::cerr << "--- shader program link log ---\n" << log << "\n";
    }
    
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        throw GLError("ShaderProgram::link(): link unsuccessful");
    }

    // show validation log if nonempty
    glGetProgramInfoLog(m_id, sizeof(log), NULL, log);
    if (log[0] != 0) {
        std::cerr << "--- shader program validation log ---\n" << log << "\n";
    }
    
    glValidateProgram(m_id);
    glGetProgramiv(m_id, GL_VALIDATE_STATUS, &success);
    if (!success) {
        throw GLError("ShaderProgram::link(): validation unsuccessful");
    }
}

void ShaderProgram::use()
{
    glUseProgram(m_id);
    GLError::check("ShaderProgram::use()");
}

GLint ShaderProgram::getAttribLocation(const std::string &name)
{
    GLint index = glGetAttribLocation(m_id, static_cast<const GLchar*>(name.c_str()));
    GLError::check("ShaderProgram::getAttribLocation()");
    
    if (index < 0) {
        throw GLError("ShaderProgram::getAttribLocation(): attribute not found: " + name);
    }

    return index;
}
