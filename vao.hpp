#ifndef VAO_HPP
#define VAO_HPP

#include "gl_ref_object.hpp"

namespace plush {
    
    /**
     * An OpenGL Vertex Array Object.
     */
    class VAO {
    protected:
        
        GLuint m_id;
        
    public:
        
        VAO();
        ~VAO();
        VAO(const VAO &src) = delete;
        VAO &operator=(const VAO &src) = delete;
        void bind();
    };
}

#endif