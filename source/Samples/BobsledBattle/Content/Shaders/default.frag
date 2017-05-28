#version 330 core

uniform float modifier;

in vec3 triangleColor;

out vec4 outColor;

void main()
{
    outColor = vec4(triangleColor * modifier, 1.0);
}
