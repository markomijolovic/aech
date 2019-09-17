#version 450 core

in vec3 world_position;

layout(location = 2) out vec4 fragment_colour;

uniform samplerCube skybox;

void main()
{
	fragment_colour = texture(skybox, world_position);	
}
