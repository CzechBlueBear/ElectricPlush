#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace plush;

Camera::Camera(const glm::vec3 &coord, float fov, float aspect)
    : m_fov(fov), m_aspect(aspect), m_coord(coord), m_azimuth(0.0f)
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
    glm::mat4 m = glm::mat4(1.0f);
    m = glm::rotate(m, -m_elevation, glm::vec3(1.0f, 0.0f, 0.0f));
    m = glm::rotate(m, m_azimuth, glm::vec3(0.0f, 1.0f, 0.0f));
    m = glm::translate(m, -m_coord);
    return m;
}

glm::mat4 Camera::projectionMatrix() const
{
    return glm::perspective(45.0f, 1.33f, 1.0f, 10.0f);
}

glm::mat4 Camera::eyeToWorldMatrix() const
{
    return glm::affineInverse(viewMatrix());
}

glm::vec3 Camera::forwardVector() const
{
    glm::mat4 eyeToWorldMatrix = glm::affineInverse(viewMatrix());
    glm::vec4 result = eyeToWorldMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    return glm::vec3(result);
}

glm::vec3 Camera::walkVector() const
{
    return glm::vec3(glm::rotate(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f), -m_azimuth, glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Camera::upload(GLuint uniform_modelMatrix, GLuint uniform_viewMatrix, GLuint uniform_projectionMatrix, GLuint uniform_normalMatrix)
{
    glm::mat4 m;
    
    m = modelMatrix();
    glUniformMatrix4fv(uniform_modelMatrix, 1, GL_FALSE, glm::value_ptr(m));
    m = viewMatrix();
    glUniformMatrix4fv(uniform_viewMatrix, 1, GL_FALSE, glm::value_ptr(m));
    m = projectionMatrix();
    glUniformMatrix4fv(uniform_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m));
    m = glm::affineInverse(viewMatrix());
    glUniformMatrix4fv(uniform_normalMatrix, 1, GL_FALSE, glm::value_ptr(m));
}
