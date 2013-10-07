#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace plush;

Camera::Camera(glm::vec3 coord, glm::vec3 target, float fov, float aspect)
    : m_coord(coord), m_target(target), m_fov(fov), m_aspect(aspect)
{
}

void Camera::setCoord(const glm::vec3 &newCoord)
{
    m_coord = newCoord;
}

glm::mat4 Camera::modelMatrix() const
{
    return glm::mat4(1.0f);
}

glm::mat4 Camera::viewMatrix() const
{
     return glm::lookAt(m_coord, m_target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::projectionMatrix() const
{
    return glm::perspective(45.0f, 1.33f, 1.0f, 10.0f);
}

glm::vec3 Camera::forwardVector() const
{
    glm::mat4 eyeToWorldMatrix = glm::affineInverse(viewMatrix());
    glm::vec4 result = eyeToWorldMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    return glm::vec3(result);
}

void Camera::upload(GLuint uniform_modelMatrix, GLuint uniform_viewMatrix, GLuint uniform_projectionMatrix)
{
    glm::mat4 m;
    
    m = modelMatrix();
    glUniformMatrix4fv(uniform_modelMatrix, 1, GL_FALSE, glm::value_ptr(m));
    m = viewMatrix();
    glUniformMatrix4fv(uniform_viewMatrix, 1, GL_FALSE, glm::value_ptr(m));
    m = projectionMatrix();
    glUniformMatrix4fv(uniform_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m));
}
