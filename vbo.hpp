#ifndef VBO_HPP
#define VBO_HPP

#include "gl.hpp"

#include <vector>

namespace plush {
 
    /**
     * A Vertex Buffer Object (holds data in OpenGL memory).
     */
    class VBO {
    protected:
        
        GLuint m_id;
        GLenum m_target;
        GLenum m_usage;
        
    public:
        
        /**
         * Constructor.
         * 
         * @arg target OpenGL target slot to bind the buffer object to (when bind() is called).
         * @arg usage Usage pattern (e.g. GL_STATIC_DRAW, GL_DYNAMIC_READ, ...)
         */
        VBO(GLenum target, GLenum usage);
        ~VBO();
        VBO(const VBO &src) = delete;
        VBO &operator =(const VBO &src) = delete;
        
        /**
         * Binds the buffer object to its target.
         */
        void bind();
        
        /**
         * Binds the buffer object to its target, resizes it as requested,
         * and fills it with data (if @a src is not null).
         * 
         * The object stays bound after the operation.
         */
        void data(size_t byteSize, const void* src);
    };
    
    /**
     * A vertex buffer object that binds to the GL_ARRAY_BUFFER target.
     * Used for all kinds of vertex attribute arrays.
     */
    class ArrayBuffer : public VBO {
    public:
        
        ArrayBuffer(GLenum usage);
    };
    
    /**
     * A vertex buffer object that binds to the GL_ELEMENT_ARRAY_BUFFER target.
     * Used for indices into vertex arrays.
     */
    class ElementArrayBuffer : public VBO {
    public:
        
        ElementArrayBuffer(GLenum usage);
    };
}

#endif