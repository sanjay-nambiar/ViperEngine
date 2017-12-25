#version 330 core

uniform float modifier;
uniform sampler2D textureSample1;
uniform sampler2D textureSample2;

in vec3 Color;
in vec2 TexCoord;

out vec4 finalColor;

void main()
{
	finalColor = texture(textureSample1, TexCoord);
	vec4 color2 = texture(textureSample2, TexCoord) * vec4(Color * modifier, 1.0);
	
	if (finalColor.x < (3 * color2.x) || finalColor.y < (3 * color2.y) || finalColor.z < (3 * color2.z))
	{
		finalColor = color2;
	}
}
