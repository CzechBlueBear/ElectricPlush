#include "vbo.hpp"
#include "gl_error.hpp"

#include <stdexcept>
#include <algorithm>
#include <iterator>

using namespace plush;

VBO::VBO(GLenum target, GLenum usage)
    : m_id(0), m_target(target), m_usage(usage)
{   
    glGenBuffers(1, &m_id);
    if (m_id == 0)
        throw GLError("VBO::VBO(): could not allocate a VBO handle");
}

VBO::~VBO()
{
    glDeleteBuffers(1, &m_id);
}

void VBO::bind()
{
    glBindBuffer(m_target, m_id);
    GLError::check("VBO::bind()");
}

void VBO::data(size_t byteSize, const void* src)
{
    bind();
    glBufferData(m_target, byteSize, src, m_usage);
    GLError::check("VBO::data()");
}

ArrayBuffer::ArrayBuffer(GLenum usage) : VBO(GL_ARRAY_BUFFER, usage)
{
}

ElementArrayBuffer::ElementArrayBuffer(GLenum usage) : VBO(GL_ELEMENT_ARRAY_BUFFER, usage)
{
}
