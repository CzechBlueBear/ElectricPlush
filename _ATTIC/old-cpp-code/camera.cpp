#include "camera.hpp"
#include "uniform.hpp"

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

void Camera::setAzimuth(float azimuth)
{
    m_azimuth = remainder(azimuth, 360.0f);
}

void Camera::setElevation(float elevation)
{
    m_elevation = remainder(elevation, 360.0f);
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

void Camera::upload(UniformMat4 &uniform_viewMatrix, UniformMat4 &uniform_projectionMatrix)
{
    glm::mat4 m;
    
    m = viewMatrix();
    uniform_viewMatrix.set(m);
    //glUniformMatrix4fv(uniform_viewMatrix, 1, GL_FALSE, glm::value_ptr(m));
    m = projectionMatrix();
    uniform_projectionMatrix.set(m);
    //glUniformMatrix4fv(uniform_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m));
}
