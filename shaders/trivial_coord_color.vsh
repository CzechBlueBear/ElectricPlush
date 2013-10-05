#version 400

in vec3 coord;
in vec3 color;
out vec3 fragColor;

void main()
{
    gl_Position = vec4(coord, 1.0);
    fragColor = color;
}
