#version 450 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_uvs;
layout (location = 2) in vec3 in_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec2 uvs;
out vec3 fragment_world_position;

void main()
{
	fragment_world_position = vec3(model * vec4(in_position, 1.0));
	normal = vec3(model * vec4(in_normal, 0.0));
	uvs = in_uvs;
	gl_Position = projection * view * model * vec4(in_position, 1.0);
}