#include "vao.hpp"
#include "vbo.hpp"
#include "gl_error.hpp"
#include "shader_program.hpp"

#include <stdexcept>

using namespace plush;

VAO::VAO() : m_id(0)
{
    glGenVertexArrays(1, &m_id);
    if (m_id == 0)
        throw GLError("VAO::VAO(): could not allocate VAO handle");
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &m_id);
}

void VAO::bind()
{
    glBindVertexArray(m_id);
}

void VAO::bindNull()
{
    glBindVertexArray(0);
}

void VAO::enableVertexAttribArray(const std::string &attribName)
{
    GLint attrib = ShaderProgram::current().getAttribLocation(attribName);
    glEnableVertexAttribArray(attrib);
}

void VAO::vertexAttribPointer(const std::string &attribName,
                              VBO &buf,
                              unsigned int valueCount, GLenum valueType,
                              bool normalize,
                              int stride, int offset)
{
    GLint attrib = ShaderProgram::current().getAttribLocation(attribName);
    buf.bind();
    glVertexAttribPointer(attrib, valueCount, valueType, normalize, stride,
                          reinterpret_cast<GLvoid*>(offset));
}


/////////////////////////////////////////////////////////////////////////////
// VAOBinder
/////////////////////////////////////////////////////////////////////////////

VAOBinder::VAOBinder(VAO& vao)
    : m_vao(vao)
{
    m_vao.bind();
}

VAOBinder::~VAOBinder()
{
    VAO::bindNull();
}
