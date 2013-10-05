#include "gl_error.hpp"
#include "gl.hpp"

#include <string>

using namespace plush;

void GLError::check(const std::string &where)
{
    GLint err = glGetError();
    if (!err)
        return;

    std::string errName;
    switch (err) {
        case GL_INVALID_ENUM:
            errName = "GL_INVALID_ENUM";
            break;
            
        case GL_INVALID_VALUE:
            errName = "GL_INVALID_VALUE";
            break;
            
        case GL_INVALID_OPERATION:
            errName = "GL_INVALID_OPERATION";
            break;
            
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errName = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
            
        case GL_OUT_OF_MEMORY:
            errName = "GL_OUT_OF_MEMORY";
            break;
        
        default:
            errName = "(unknown GL error)";
            break;
    }
    
    throw GLError(where + ": " + errName);
}
