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
        float m_elevation;      ///< Elevation (in degrees).
        
    public:
        
        Camera(glm::vec3 coord, glm::vec3 target, float fov=45.0f, float aspect=1.33f);
        
        const glm::vec3 &getCoord() const { return m_coord; }
        void setCoord(const glm::vec3 &newCoord);
        float getAzimuth() const { return m_azimuth; }
        void setAzimuth(float azimuth) { m_azimuth = azimuth; }
        float getElevation() const { return m_elevation; }
        void setElevation(float elevation) { m_elevation = elevation; }
        
        glm::mat4 modelMatrix() const;
        glm::mat4 viewMatrix() const;
        glm::mat4 projectionMatrix() const;
        
        /**
         * Returns the forward vector: points to where the camera looks.
         */
        glm::vec3 forwardVector() const;
        
        /**
         * Returns the walk vector, used for walking forward on the horizontal plane
         * (not affected by elevation).
         */
        glm::vec3 walkVector() const;

        /// Uploads the camera matrices to shader uniforms.
        void upload(GLuint uniform_modelMatrix, GLuint uniform_viewMatrix, GLuint uniform_projectionMatrix);
    };
}

#endif