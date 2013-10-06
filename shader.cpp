#include "shader.hpp"
#include "gl_error.hpp"

#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>

#include "gl.hpp"

using namespace plush;

Shader::Shader(GLenum shaderType) : m_id(0)
{
    m_id = glCreateShader(shaderType);
    GLError::check("Shader::Shader()");
}

Shader::Shader(GLenum shaderType, const std::string &filename) : Shader(shaderType)
{
    load(filename);
}

Shader::~Shader()
{
    glDeleteShader(m_id);
    m_id = 0;
}

void Shader::loadFromString(const std::string &text)
{
    const char *textPointers[1] = { text.c_str() };
    const GLint lengths[1] = { static_cast<GLint>(text.length()) };
    
    glShaderSource(m_id, 1, textPointers, lengths);
    GLError::check("Shader::loadFromString(): after glShaderSource()");

    glCompileShader(m_id);
    GLError::check("Shader::loadFromString(): after glCompileShader()");

    // even if compilation succeeds, the log can still contain warnings
    // so show it every time it's not empty
    GLchar log[1024];
    glGetShaderInfoLog(m_id, 1024, NULL, log);
    if (log[0] != '\0') {
        std::cerr << log << "\n";
    }
    
    GLint success;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        throw GLError("Shader::loadFromString(): compilation failed");
    }
}

void Shader::load(const std::string &filename)
{
    std::fstream in;
    in.open(filename, std::ios_base::in | std::ios_base::binary);
    
    if (!in.is_open())
        throw GLError("Shader::load(): could not open file: " + filename);

    std::vector<char> buf;
    buf.reserve(1024);
    for (;;) {
        int c = in.get();
        if (c < 0) break;
        buf.push_back(static_cast<char>(c));
    }
    
    if (in.fail() && !in.eof())
        throw GLError("Shader::load(): error reading file: " + filename);
    
    // add terminating zero to make a valid C-style string
    buf.push_back('\0');

    loadFromString(buf.data());
}

FragmentShader::FragmentShader() : Shader(GL_FRAGMENT_SHADER)
{
}

FragmentShader::FragmentShader(const std::string &filename): Shader(GL_FRAGMENT_SHADER, filename)
{
}

VertexShader::VertexShader(): Shader(GL_VERTEX_SHADER)
{
}

VertexShader::VertexShader(const std::string &filename): Shader(GL_VERTEX_SHADER, filename)
{
}
