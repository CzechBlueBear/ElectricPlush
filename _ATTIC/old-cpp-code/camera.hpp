#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "gl.hpp"
#include <glm/glm.hpp>

namespace plush {
    
    class UniformMat4;
    
    /**
     * Describes the position, orientation and other properties of a camera.
     */
    class Camera {
    protected:
        
        /// Field of view (in degrees).
        float m_fov = 45.0f;
        
        /// Aspect ratio of the viewport.
        float m_aspect = 1.33f;

        /// Coordinates of the camera in world space.
        glm::vec3 m_coord = glm::vec3(0, 0, 0);
        
        /// Azimuth (in degrees, deviation from north, clockwise) in world space.
        float m_azimuth = 0.0f;
        
        /// Elevation (in degrees, 0 is horizontal, + means up, - is down).
        float m_elevation = 0.0f;
        
    public:
        
        Camera() { }
        Camera(const glm::vec3 &coord, float fov=45.0f, float aspect=1.33f);
        
        const glm::vec3 &getCoord() const { return m_coord; }
        void setCoord(const glm::vec3 &newCoord);
        float getAzimuth() const { return m_azimuth; }
        
        /// Sets the azimuth (value is clamped to <-180, 180)).
        void setAzimuth(float azimuth);
        float getElevation() const { return m_elevation; }
        
        /// Sets the elevation (value clamped to <-180, 180)).
        void setElevation(float elevation);
        
        glm::mat4 viewMatrix() const;
        glm::mat4 projectionMatrix() const;
        
        /// Returns the eye-to-world (inverse to view) matrix.
        glm::mat4 eyeToWorldMatrix() const;

        /// Returns the forward vector: points to where the camera looks.
        glm::vec3 forwardVector() const;
        
        /**
         * Returns the walk vector, used for walking forward on the horizontal plane
         * (not affected by elevation).
         */
        glm::vec3 walkVector() const;

        /// Uploads the camera matrices to shader uniforms.
        void upload(UniformMat4 &uniform_viewMatrix, UniformMat4 &uniform_projectionMatrix);
    };
}

#endif