#version 330

layout (location = 0) in vec3 position;

uniform mat4 _world;

void main()
{
	gl_Position = _world * vec4(position, 1.0);
}