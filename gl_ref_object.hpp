#ifndef GL_REF_OBJECT_HPP
#define GL_REF_OBJECT_HPP

#include "gl.hpp"

namespace plush {

    /// An objectified non-owning reference to an OpenGL object.
    class GLRefObject {
    protected:
        
        /// OpenGL identifier of the referenced object.
        /// 0 is a predefined empty value.
        GLuint m_id;
        
    public:
        
        /// Constructor.
        /// The reference is set to an empty value.
        GLRefObject() : m_id(0) { }
        
        /// Constructor, sets an explicit OpenGL object ID.
        /// The ID does not have to be valid in the moment of construction.
        GLRefObject(GLuint id) : m_id(id) { }

        /// Destructor.
        /// Does *not* destroy the referenced OpenGL object.
        virtual ~GLRefObject() { }
                
        /// Returns the OpenGL identifier of the referenced object,
        /// or 0 if this is an empty reference.
        GLuint id() { return m_id; }
        
        /// Deletes the referenced OpenGL object and resets the stored ID to 0.
        /// (For some OpenGL objects, like shaders, the referenced object might
        /// not be deleted immediately.)
        /// It is not an error to call del() for an empty reference.
        virtual void del() = 0;
    };
}

#endif