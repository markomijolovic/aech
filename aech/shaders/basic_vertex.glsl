#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uvs;
layout(location = 2) in vec3 in_normal;

out vec3 position;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat4 normal_matrix = transpose(inverse(model));
	normal = vec3(normal_matrix * vec4(in_normal, 0.0F));
	position = vec3(model * vec4(in_position, 1.0F));

	gl_Position = projection * view * model * vec4(in_position, 1.0F);
}
