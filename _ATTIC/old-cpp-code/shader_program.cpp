#include "complain.hpp"
#include "shader_program.hpp"
#include "gl_error.hpp"

#include <stdexcept>
#include <iostream>

using namespace plush;

/////////////////////////////////////////////////////////////////////////////
// ShaderProgramProxy
/////////////////////////////////////////////////////////////////////////////

void ShaderProgramProxy::use()
{
    glUseProgram(m_id);
    ON_GL_ERROR_COMPLAIN();
}

GLint ShaderProgramProxy::getAttribLocation(const std::string &name)
{
    GLint index = glGetAttribLocation(m_id, static_cast<const GLchar*>(name.c_str()));
    
    // Note: No complaints will be printed if the attribute is unknown;
    // this is not considered a GL error.
    // This check will catch errors like invalid program ID etc.
    ON_GL_ERROR_COMPLAIN();

    return index;
}

GLint ShaderProgramProxy::getUniformLocation(const std::string &name)
{
    GLint index = glGetUniformLocation(m_id, static_cast<const GLchar*>(name.c_str()));

    // Note: No complaints will be printed if the uniform is unknown;
    // this is not considered a GL error.
    // This check will catch errors like invalid program ID etc.
    ON_GL_ERROR_COMPLAIN();

    return index;
}

/////////////////////////////////////////////////////////////////////////////
// ShaderProgram
/////////////////////////////////////////////////////////////////////////////

ShaderProgram::ShaderProgram()
{
    m_id = glCreateProgram();
    ON_GL_ERROR_COMPLAIN();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_id);
}

void ShaderProgram::attach(Shader &sh)
{
    glAttachShader(m_id, sh.m_id);
    ON_GL_ERROR_COMPLAIN();
}

bool ShaderProgram::link()
{
    glLinkProgram(m_id);
    ON_GL_ERROR_COMPLAIN();

    GLint success = 0;
    GLchar log[1024] = { 0 };

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
        COMPLAIN("link failed");

    // show link log if nonempty (it can contain warnings even if link succeeds)
    glGetProgramInfoLog(m_id, sizeof(log), NULL, log);
    if (log[0] != 0)
        std::cerr << "--- shader program link log ---\n" << log << "\n";

    if (!success)
        return false;
    
    glValidateProgram(m_id);
    glGetProgramiv(m_id, GL_VALIDATE_STATUS, &success);
    if (!success)
        COMPLAIN("validation failed");

    // show validation log if nonempty
    glGetProgramInfoLog(m_id, sizeof(log), NULL, log);
    if (log[0] != 0)
        std::cerr << "--- shader program validation log ---\n" << log << "\n";
    
    if (!success)
        return false;
    
    return true;
}

void ShaderProgram::use()
{
    ShaderProgramProxy(*this).use();
}

void ShaderProgram::useNull()
{
    glUseProgram(0);
}

GLint ShaderProgram::getAttribLocation(const std::string &name)
{
    return ShaderProgramProxy(*this).getAttribLocation(name);
}

GLint ShaderProgram::getUniformLocation(const std::string &name)
{
    return ShaderProgramProxy(*this).getUniformLocation(name);
}

ShaderProgramProxy ShaderProgram::current()
{
    GLint id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &id);
    return ShaderProgramProxy(id);
}
