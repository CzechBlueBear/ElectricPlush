#version 330

layout (location = 0) in vec3 vertexCoord;
layout (location = 1) in vec3 vertexColor;
out vec3 color;

void main()
{
    gl_Position = vec4(vertexCoord, 1.0);
    color = vertexColor;
}
