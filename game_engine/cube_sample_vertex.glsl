#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in ve2 uvs;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 world_position;

void main()
{
	vec4 world_pos = model * (vec4, position, 1.0);
	world_position.xyz = world_pos.xyz;


}
