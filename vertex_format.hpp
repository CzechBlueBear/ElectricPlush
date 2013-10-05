#ifndef VERTEX_FORMAT_HPP
#define VERTEX_FORMAT_HPP

#include <glm/glm.hpp>

namespace plush {
    
    /// A vertex structure that has only two elements: a position in 3d,
    /// and a color.
    struct Vertex_pos_color {
        glm::vec3 pos;
        glm::vec3 color;
    };
}

#endif