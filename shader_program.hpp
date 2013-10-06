#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include "gl.hpp"
#include "gl_ref_object.hpp"
#include "shader.hpp"

namespace plush {
 
    /// Reference to a shader program in OpenGL memory.
    class ShaderProgram  {
    protected:
        
        GLuint m_id;
        
    public:
        
        ShaderProgram();
        ShaderProgram(const ShaderProgram &prog) = delete;
        ShaderProgram &operator=(const ShaderProgram &prog) = delete;
        ~ShaderProgram();

        /// Attaches a shader to the program.
        /// After attaching all shaders, link() must be called.
        void attach(Shader &sh);
        
        /// Links the program.
        /// Throws a runtime_error if the link fails.
        void link();
        
        /// Makes the program the active program in the current GL context.
        void use();
        
        /// Returns the index of the named attribute in the program.
        GLint getAttribLocation(const std::string &name);
    };
    
}

#endif