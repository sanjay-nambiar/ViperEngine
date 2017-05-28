#version 330 core
  
in vec2 position;
in vec3 color;

out vec3 triangleColor;

void main()
{
	triangleColor = color;
    gl_Position = vec4(position, 0.0, 1.0);
}
