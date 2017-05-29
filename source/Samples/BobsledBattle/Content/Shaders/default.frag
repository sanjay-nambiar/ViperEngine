#version 330 core

uniform float modifier;
uniform sampler2D textureSample1;
uniform sampler2D textureSample2;

in vec3 Color;
in vec2 TexCoord;

out vec4 finalColor;

void main()
{
	finalColor = mix(texture(textureSample1, TexCoord), texture(textureSample2, TexCoord) * vec4(Color * modifier, 1.0), 0.65);
}
