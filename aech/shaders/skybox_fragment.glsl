#version 460 core

in vec3 world_position;

out vec4 fragment_colour;

uniform samplerCube skybox;

void main()
{
	fragment_colour=vec4(0.6,0.6,0.6,1);
	//fragment_colour = texture(skybox, world_position);	
}
