#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace plush;

Camera::Camera(glm::vec3 coord, glm::vec3 target, float fov, float aspect)
    : m_coord(coord), m_target(target), m_fov(fov), m_aspect(aspect)
{
}

glm::mat4 Camera::modelMatrix() const
{
    return glm::mat4(1.0f);
}

glm::mat4 Camera::viewMatrix() const
{
    return glm::lookAt(m_coord, m_target, glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::mat4 Camera::projectionMatrix() const
{
    return glm::perspective(45.0f, 1.33f, 1.0f, 10.0f);
}
