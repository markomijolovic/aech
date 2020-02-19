#version 460 core

layout(location = 0) in vec3 in_position;

out vec3 fragment_world_position;
out vec4 screen_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fragment_world_position = vec3(model * vec4(in_position, 1.0));
	screen_position = projection * view * model * vec4(in_position, 1.0);
	gl_Position = projection * view * model * vec4(in_position, 1.0);
}
