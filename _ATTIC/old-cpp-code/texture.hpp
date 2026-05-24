#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "gl_object.hpp"
#include "bindable.hpp"

namespace plush {
    
    /**
     * A texture.
     */
    class Texture : public GLObject, public GLBindable {
    public:
       
        /**
         * Various texture targets, as defined in OpenGL specs.
         * 
         * The target determines what type of state and dimensionality
         * the texture represents, and each target is a separate slot
         * regarding call to bind().
         */
        enum Target {
            TEXTURE_1D = GL_TEXTURE_1D,
            TEXTURE_2D = GL_TEXTURE_2D,
            TEXTURE_3D = GL_TEXTURE_3D,
            TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
            TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
            TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
            TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
            TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
            TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
            TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
        };
        
    protected:
        
        /// Target of the texture.
        Target m_target;
    
    public:

        Texture(Target type);
        ~Texture();
        
        /**
         * Binds the texture to its target (replacing any previous)
         * in the currently active texturing unit.
         */
        void bind() override;
        
        /**
         * Unbinds the texture from its target in the currently active
         * texturing unit.
         */
        void unbind() override;
        
        /**
         * Sets the currently active texturing unit.
         * 
         * @num Texturing unit number, starting from 0 up to
         * an implementation-defined limit which is always at least 79.
         */
        static void setActiveTexturingUnit(int num);
    };
    
} // namespace

#endif