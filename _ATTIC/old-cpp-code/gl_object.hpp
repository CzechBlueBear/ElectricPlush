#ifndef GL_OBJECT_HPP
#define GL_OBJECT_HPP

#include "gl.hpp"

namespace plush {
    
    /**
     * Base class for objects that represent OpenGL objects.
     * 
     * These objects are owners: when they are created, the appropriate GL object
     * is created (or allocated, according to type), and when they are destroyed,
     * the GL object is also destroyed (or its reference count decremented).
     */
    class GLObject {
    protected:
        
        /**
         * A numeric identifier of the OpenGL object.
         * 
         * OpenGL standard specifies that 0 is always an invalid identifier.
         * 
         * OpenGL standard is not always clear whether various types of GL objects
         * have separate numeric namespaces so do not rely on the identifier being
         * unique between various types of objects.
         */
        GLuint m_id;

        /**
         * Default constructor. Initializes m_id to 0 (invalid).
         */
        GLObject() : m_id(0) { }
        
        /**
         * Destructor. The GL object should be destroyed or its reference count
         * decreased here.
         */
        virtual ~GLObject();
        
    public:
        
        /**
         * Copy constructor; deleted as its behavior would be hard to define
         * (the underlying GL object typically is not simply copyable).
         */
        GLObject(const GLObject &src) = delete;
        
        /**
         * Assignment operator; deleted as its behavior would be hard to define.
         */
        GLObject &operator=(const GLObject &src) = delete;
        
        /**
         * Returns the numeric identifier of the underlying OpenGL object.
         * 
         * @important Use this call with care. It bypasses encapsulation
         * and the caller has responsibility to keep the OpenGL object
         * in the state the wrapper expects it to be.
         */
        GLuint id() { return m_id; }
    };

    /**
     * A non-owning, copyable, passable-by-value reference to an OpenGL object.
     * 
     * Multiple proxies can exist for the same OpenGL object, and can be copied
     * and reassigned as any primitive value.
     * 
     * Each proxy type has an appropriate non-proxy GLObject type.
     */
    template <typename T>
    class GLObjectProxy {
    protected:
        
        /**
         * A numeric identifier of the underlying OpenGL type.
         */
        GLuint m_id;

        /**
         * Constructor. Makes a proxy for the OpenGL object with the given
         * identifier.
         */
        GLObjectProxy(GLuint id) : m_id(id) { }

    public:

        /**
         * Constructor. The proxy is initialized to an empty value.
         */
        GLObjectProxy() : m_id(0) { }
        
        /**
         * Constructor. Makes a proxy for the specified GLObject.
         */
        GLObjectProxy(T &src) : m_id(src.id()) { }
        
        /**
         * Destructor (empty).
         */
        ~GLObjectProxy() { }

        /**
         * Returns true if the proxy is null, i.e. does not refer to any
         * OpenGL object (its m_id is 0).
         */
        bool isNull() const { return (m_id == 0); }
    };
    
} // namespace

#endif