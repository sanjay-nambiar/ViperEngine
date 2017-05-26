#version 330 core

uniform float modifier;

out vec4 outColor;
in vec3 vertexColor;

void main()
{
    outColor = vec4(mod((vertexColor.x + modifier), 1),
		mod((vertexColor.y + modifier), 1),
		mod((vertexColor.z + modifier), 1),
		1.0f);
}
