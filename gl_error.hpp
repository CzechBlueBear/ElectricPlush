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
}

#endif