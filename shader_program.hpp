#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include "gl.hpp"
#include "shader.hpp"

namespace plush {

    /// A copyable, pass-by-value-able reference to an OpenGL shader program.
    class ShaderProgramProxy {
    protected:
        
        /// The OpenGL identifier of the program.
        GLuint m_id;
    
        /// Constructor (the OpenGL identifier is passed as an argument).
        ShaderProgramProxy(GLuint id) : m_id(id) { };
    
        friend class ShaderProgram;
        
    public:
        
        ShaderProgramProxy(const ShaderProgramProxy &src) = default;
        ShaderProgramProxy &operator=(const ShaderProgramProxy &src) = default;
        ~ShaderProgramProxy() = default;

        /// Returns true for a null proxy (which does not refer to any program).
        bool isNull() { return (m_id == 0); }
        
        /// Equivalent to ShaderProgram::use().
        void use();
        
        /// Equivalent to ShaderProgram::getAttribLocation().
        GLint getAttribLocation(const std::string &name);

        /// Equivalent to ShaderProgram::getUniformLocation().
        GLint getUniformLocation(const std::string &name);
    };
    
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

        /**
         * Attaches a shader to the program.
         * 
         * In order to make a functional program, the shaders must be attached
         * first, then link() must be called.
         * 
         * When a shader is added to a linked program, the program must
         * be relinked to be usable again.
         */
        void attach(Shader &sh);
        
        /// Links the program.
        /// Throws a GLError if the link fails.
        void link();
        
        /// Makes the program current in the current GL context.
        void use();
        
        /// Detaches the current program without setting any other.
        static void useNull();
        
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

        /// Returns a proxy representing the current program.
        static ShaderProgramProxy current();
        
        /// Returns a proxy representing this program.
        ShaderProgramProxy proxy();
    };
    
}

#endif