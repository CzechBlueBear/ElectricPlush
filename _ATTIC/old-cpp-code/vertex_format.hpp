#ifndef VERTEX_FORMAT_HPP
#define VERTEX_FORMAT_HPP

#include <glm/glm.hpp>

namespace plush {
    
    /**
     * A vertex description to be used with simple texturing.
     * It contains a coordinate, a normal, and a texture coordinate.
     */
    struct TexturedVertex {
        glm::vec3 coord;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };
}

#endif