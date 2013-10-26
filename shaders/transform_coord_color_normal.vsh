#version 330

layout (location = 0) in vec3 vertexCoord;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;
out vec3 color;
out vec3 normal;

uniform mat4 modelMatrix = mat4(1.0);
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 normalMatrix = mat4(1.0);

void main()
{
    gl_Position = projectionMatrix*viewMatrix*modelMatrix*vec4(vertexCoord, 1.0);
    color = vertexColor;
    normal = vec3(normalMatrix*vec4(vertexNormal, 0.0));
}
