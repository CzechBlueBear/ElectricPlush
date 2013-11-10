#ifndef VBO_HPP
#define VBO_HPP

#include "gl_object.hpp"
#include "bindable.hpp"

#include <string>

namespace plush {
 
    /**
     * A Vertex Buffer Object (holds data in OpenGL memory).
     */
    class VBO : public GLObject, public GLBindable {
    protected:
        
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
        
        void bind();
        void unbind();
        
        /**
         * Binds the buffer object to its target, resizes it as requested,
         * and fills it with data (if @a src is not null).
         * 
         * The VBO stays bound after the call returns.
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

        /**
         * Binds the array buffer and establishes a link between
         * the buffer contents and the given vertex attribute
         * (using the current shader program).
         * 
         * The buffer stays bound after the call returns.
         * It can be unbound any time without affecting the link between
         * the buffer and the attribute.
         */
        void vertexAttribPointer(const std::string &attribName,
                                 unsigned int valueCount, GLenum valueType,
                                 bool normalize,
                                 int stride, int offset);
    };
    
    typedef ArrayBuffer VertexAttribBuffer;
    
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