#version 330 core
  
in vec3 position;
out vec3 vertexColor;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
	vertexColor = vec3(position.x - position.y, -(position.x + position.y), position.y + 0.5f);
}
