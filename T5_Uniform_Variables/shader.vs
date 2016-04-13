#version 330

layout (location = 0) in vec3 position;

uniform float _scale;

void main()
{
	gl_Position = vec4(position.x * _scale, position.y * _scale, position.z, 1.0);
}