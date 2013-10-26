#include "complain.hpp"
#include "gl.hpp"

#include <iostream>

void doComplain(const char *where, const std::string &msg)
{
    std::cerr << where << ": " << msg << '\n';
}

void doOnGLErrorComplain(const char *where)
{
    for (;;) {
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

        std::cerr << where << ": " << errName << '\n';
    }
}
