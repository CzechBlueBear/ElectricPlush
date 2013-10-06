#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include "gl.hpp"
#include "gl_ref_object.hpp"
#include "shader.hpp"

namespace plush {

    /**
     * An OpenGL shader program.
     */
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
        
        /**
         * Returns the index of the named attribute in the program.
         * Throws NoSuchAttrib if the attribute is not found.
         */
        GLint getAttribLocation(const std::string &name);

        /**
         * Returns the index of the uniform in the program.
         * Throws NoSuchUniform if the uniform is not found.
         */
        GLint getUniformLocation(const std::string &name);
    };
    
}

#endif