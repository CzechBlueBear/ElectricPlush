#ifndef SHADER_HPP
#define SHADER_HPP

#include "gl.hpp"
#include "gl_ref_object.hpp"

#include <string>

namespace plush {
 
    /**
     * An OpenGL shader.
     */
    class Shader {
    protected:
        
        GLuint m_id;
        
        friend class ShaderProgram;

        Shader(GLenum shaderType);
        Shader(GLenum shaderType, const std::string &filename);
    
    public:
        
        Shader(const Shader &src) = delete;
        Shader &operator=(const Shader &src) = delete;

        ~Shader();

        /**
         * Loads and compiles the shader from the source string.
         * Previous shader binary is removed.
         * Throws GLError on failure (typically when the source has errors).
         */
        void loadFromString(const std::string &text);
        
        /**
         * Loads and compiles the shader from the source file.
         * Previous shader binary is removed.
         */
        void load(const std::string &filename);
    };

    class FragmentShader : public Shader {
    public:
        
        FragmentShader();
        FragmentShader(const std::string &filename);
    };
    
    class VertexShader : public Shader {
    public:
        
        VertexShader();
        VertexShader(const std::string &filename);
    };
}

#endif