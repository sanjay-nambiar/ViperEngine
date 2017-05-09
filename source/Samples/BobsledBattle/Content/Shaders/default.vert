#version 330 core
  
layout (location = 0) in vec3 position;
out vec4 vertexColor;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
	vertexColor = vec4(position.x - position.y, -(position.x + position.y), position.y + 0.5f, 1.0);
}
