#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "gl.hpp"
#include <glm/glm.hpp>

namespace plush {
    
    class Camera {
    protected:
        
        float m_fov;            ///< Field of view (in degrees).
        float m_aspect;         ///< Aspect ratio of the viewport.
        glm::vec3 m_coord;      ///< Coordinates of the camera in world space.
        glm::vec3 m_target;     ///< Target (point the camera looks at).
        float m_azimuth;        ///< Azimuth (in degrees) in world space.
        
    public:
        
        Camera(glm::vec3 coord, glm::vec3 target, float fov=45.0f, float aspect=1.33f);
        
        const glm::vec3 &getCoord() const { return m_coord; }
        void setCoord(const glm::vec3 &newCoord);
        float getAzimuth() const { return m_azimuth; }
        void setAzimuth(float azimuth) { m_azimuth = azimuth; }
        
        glm::mat4 modelMatrix() const;
        glm::mat4 viewMatrix() const;
        glm::mat4 projectionMatrix() const;
        
        glm::vec3 forwardVector() const;
        
        /// Uploads the camera matrices to shader uniforms.
        void upload(GLuint uniform_modelMatrix, GLuint uniform_viewMatrix, GLuint uniform_projectionMatrix);
    };
}

#endif