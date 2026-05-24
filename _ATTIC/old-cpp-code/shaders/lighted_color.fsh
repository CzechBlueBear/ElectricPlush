#version 330

in vec3 color;
in vec3 normal;
out vec4 fragColor;

uniform vec3 hollywoodLight;

void main(void)
{
    float hollywoodLightFactor = dot(-normal, normalize(hollywoodLight));

    fragColor = (0.9 + 0.1*hollywoodLightFactor)*vec4(color, 1.0f);
}
