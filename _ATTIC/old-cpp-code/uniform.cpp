#include "gl.hpp"
#include "uniform.hpp"
#include "shader_program.hpp"

#include <glm/gtc/type_ptr.hpp>

using namespace plush;

Uniform::Uniform(const std::string &name)
    : m_name(name), m_index(-1)
{
}
    
bool Uniform::find()
{
    m_index = ShaderProgram::current().getUniformLocation(m_name);
    return (m_index >= 0);
}

void UniformVec3::set(const glm::vec3 &v)
{
    glUniform3fv(m_index, 1, glm::value_ptr(v));
}

void UniformMat4::set(const glm::mat4 &m)
{
    glUniformMatrix4fv(m_index, 1, GL_FALSE, glm::value_ptr(m));
}
