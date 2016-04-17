#version 330

layout (location = 0) in vec3 position;

uniform mat4 _World;

void main()
{
	gl_Position = _World * vec4(position.x, position.y, position.z, 1.0);
}