#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include "gl_object.hpp"
#include "shader.hpp"

namespace plush {

    class ShaderProgramProxy;

    /**
     * An OpenGL shader program.
     */
    class ShaderProgram : public GLObject {
    public:
        
        ShaderProgram();
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
        
        /**
         * Links the program.
         * Returns true on success, false on failure. Errors or warnings
         * from the compilation are printed on standard error output.
         */
        bool link();
        
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

    /// A copyable, pass-by-value-able reference to an OpenGL shader program.
    class ShaderProgramProxy : public GLObjectProxy<ShaderProgram> {
    protected:
        
        friend ShaderProgram;
        
        ShaderProgramProxy(GLuint id) : GLObjectProxy<ShaderProgram>(id) { };
        
    public:

        ShaderProgramProxy(ShaderProgram &src) : GLObjectProxy<ShaderProgram>(src) { };
        
        /// Equivalent to ShaderProgram::use().
        void use();
        
        /// Equivalent to ShaderProgram::getAttribLocation().
        GLint getAttribLocation(const std::string &name);

        /// Equivalent to ShaderProgram::getUniformLocation().
        GLint getUniformLocation(const std::string &name);
    };
        
}

#endif