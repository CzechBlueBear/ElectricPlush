#ifndef GL_ERROR_HPP
#define GL_ERROR_HPP

#include "gl.hpp"

#include <string>
#include <stdexcept>

namespace plush {
    
    /**
     * An OpenGL error.
     */
    class GLError : public std::runtime_error {
    public:
        GLError(const std::string &what_arg) : std::runtime_error(what_arg) { }
        GLError(const char *what_arg) : std::runtime_error(what_arg) { }
       
        /** 
         * Checks the OpenGL error flag, throwing a GLError if set,
         * otherwise returns normally.
         *
         * @arg where Identifies where in the code the error occurred.
         */
        static void check(const std::string &where);
    };
    
    /// Thrown when a named attribute is not found in a shader program.
    class NoSuchAttrib: public GLError {
    public:
        NoSuchAttrib(const std::string &attribName);
    };
    
    /// Thrown when a named uniform is not found in a shader program.
    class NoSuchUniform : public GLError {
    public:
        NoSuchUniform(const std::string &uniformName);
    };
}

#endif