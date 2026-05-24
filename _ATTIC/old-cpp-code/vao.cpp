#include "complain.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include "gl_error.hpp"
#include "shader_program.hpp"

#include <stdexcept>

using namespace plush;

VAO::VAO()
{
    glGenVertexArrays(1, &m_id);
    ON_GL_ERROR_COMPLAIN();
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &m_id);
}

void VAO::bind()
{
    glBindVertexArray(m_id);
}

void VAO::unbind()
{
    glBindVertexArray(0);
}

void VAO::enableVertexAttribArray(const std::string &attribName)
{
    GLint attrib = ShaderProgram::current().getAttribLocation(attribName);
    if (attrib < 0) {
        
        // attribute not found, was reported earlier, just return
        return;
    }

    glEnableVertexAttribArray(attrib);
}

void VAO::vertexAttribPointer(const std::string &attribName,
                              VBO &buf,
                              unsigned int valueCount, GLenum valueType,
                              bool normalize,
                              int stride, int offset)
{
    GLint attrib = ShaderProgram::current().getAttribLocation(attribName);
    if (attrib < 0 ) {
        
        // attribute not found, was reported earlier, just return
        return;
    }

    buf.bind();
    glVertexAttribPointer(attrib, valueCount, valueType, normalize, stride,
                          reinterpret_cast<GLvoid*>(offset));
    ON_GL_ERROR_COMPLAIN();
}
