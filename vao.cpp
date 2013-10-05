#include "vao.hpp"
#include "gl_error.hpp"

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
    GLError::check("VAO::bind()");
}
