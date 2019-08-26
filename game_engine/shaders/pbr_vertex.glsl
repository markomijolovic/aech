#version 450 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_uvs;
layout (location = 2) in vec3 in_normal;

out vec3 out_normal;
out vec2 out_uvs;
out vec3 out_fragment_world_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	out_fragment_world_position = vec3(model * vec4(in_position, 1.0));
	out_normal = vec3(transpose(inverse(model)) * vec4(in_normal, 0.0));
	out_uvs = in_uvs;
	gl_Position = projection * view * model * vec4(in_position, 1.0);
}
