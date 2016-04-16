#version 330

layout (location = 0) in vec3 position;

out vec4 Color;

void main()
{
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
	Color = vec4(clamp(position, 0.0, 1.0), 1.0);
}