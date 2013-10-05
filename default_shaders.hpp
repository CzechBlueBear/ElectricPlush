#ifndef DEFAULT_SHADERS_HPP
#define DEFAULT_SHADERS_HPP

namespace plush {
    
    /// Shader source for vertex shader.
    /// This shader accepts one argument, "pos" (vec3), and sends it
    /// unchanged to the fragment shader.
    extern const char *pos_passthrough_vshader;
    
    /// Shader source for fragment shader.
    /// This shader accepts one argument, "color" (vec3) and sends it
    /// unchanged to the output.
    extern const char *color_passthrough_fshader;
    
}

#endif