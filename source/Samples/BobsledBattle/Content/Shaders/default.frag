#version 330 core

uniform float modifier;
uniform sampler2D textureSample;

in vec3 Color;
in vec2 TexCoord;

out vec4 finalColor;

void main()
{
    finalColor = texture(textureSample, TexCoord) * vec4(Color * modifier, 1.0);
}
