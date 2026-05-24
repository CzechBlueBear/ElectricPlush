#include "complain.hpp"
#include "vbo.hpp"
#include "gl_error.hpp"
#include "shader_program.hpp"

#include <stdexcept>
#include <algorithm>
#include <iterator>

using namespace plush;

VBO::VBO(GLenum target, GLenum usage)
    : m_target(target), m_usage(usage)
{   
    glGenBuffers(1, &m_id);
    ON_GL_ERROR_COMPLAIN();
}

VBO::~VBO()
{
    glDeleteBuffers(1, &m_id);
}

void VBO::bind()
{
    glBindBuffer(m_target, m_id);
    ON_GL_ERROR_COMPLAIN();
}

void VBO::unbind()
{
    glBindBuffer(m_target, 0);
    ON_GL_ERROR_COMPLAIN();
}

void VBO::data(size_t byteSize, const void* src)
{
    bind();
    glBufferData(m_target, byteSize, src, m_usage);
    ON_GL_ERROR_COMPLAIN();
}

/////////////////////////////////////////////////////////////////////////////
// ArrayBuffer
/////////////////////////////////////////////////////////////////////////////

ArrayBuffer::ArrayBuffer(GLenum usage) : VBO(GL_ARRAY_BUFFER, usage)
{
}

/////////////////////////////////////////////////////////////////////////////
// ElementArrayBuffer
/////////////////////////////////////////////////////////////////////////////

ElementArrayBuffer::ElementArrayBuffer(GLenum usage) : VBO(GL_ELEMENT_ARRAY_BUFFER, usage)
{
}
