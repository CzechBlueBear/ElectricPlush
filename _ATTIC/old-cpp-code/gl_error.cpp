#include "gl_error.hpp"
#include "gl.hpp"
#include "shader.hpp"

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

NoSuchAttrib::NoSuchAttrib(const std::string &attribName)
    : GLError("No such attribute: " + attribName)
{
}

NoSuchUniform::NoSuchUniform(const std::string& uniformName)
    : GLError("No such uniform: " + uniformName)
{
}

ShaderProgramLinkError::ShaderProgramLinkError(const std::string &log)
    : GLError("Shader program link error: " + log)
{
}

ShaderProgramValidationError::ShaderProgramValidationError(const std::string &log)
    : GLError("Shader program validation error: " + log)
{
}
    