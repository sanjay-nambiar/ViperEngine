#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 Color;
out vec2 TexCoord;

void main()
{
	TexCoord = texCoord;
	Color = color;
    gl_Position = vec4(position, 0.0, 1.0);
}
