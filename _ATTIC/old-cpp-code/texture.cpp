#include "texture.hpp"

using namespace plush;

Texture::Texture(Texture::Target target)
    : m_target(target)
{
    glGenTextures(1, &m_id);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

void Texture::bind()
{
    glBindTexture(m_target, m_id);
}

void Texture::unbind()
{
    glBindTexture(m_target, 0);
}

void Texture::setActiveTexturingUnit(int num)
{
    glActiveTexture(GL_TEXTURE0 + num);
}
