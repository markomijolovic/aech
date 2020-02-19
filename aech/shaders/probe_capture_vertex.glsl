#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 in_uvs;
layout(location = 2) in vec3 in_normal;

out vec2 uvs;
out vec3 fragment_position;
out vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	uvs = in_uvs;
	normal = vec3(transpose(inverse(model)) * vec4(normal, 0.0));
	fragment_position = vec3(model * vec4(position, 1.0));

	gl_Position = projection * view * vec4(fragment_position, 1.0);
}
