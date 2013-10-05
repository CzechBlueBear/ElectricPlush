#include "default_shaders.hpp"

using namespace plush;

const char *plush::pos_passthrough_vshader =
    "#version 400 \n"
    " \n"
    "in vec3 coord; \n"
    "in vec3 color; \n"
    "out vec3 fragColor; \n\n"
    "void main() \n"
    "{ \n"
    "    gl_Position = vec4(coord, 1.0); \n"
    "    fragColor = color; \n"
    "} \n"
    ;

const char *plush::color_passthrough_fshader =
    "#version 400 \n"
    " \n"
    "in vec3 fragColor; \n"
    "out vec4 outColor; \n"
    " \n"
    "void main() \n"
    "{ \n"
    "    outColor = vec4(fragColor, 1.0); \n"
    "} \n"
    ;
