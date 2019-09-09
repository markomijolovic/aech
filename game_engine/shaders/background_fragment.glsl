#version 450 core

in vec3 world_position;

out vec4 fragment_colour;

uniform samplerCube environment_map;

void main()
{
	vec3 environment_colour = texture(environment_map, world_position).rgb;

	environment_colour = environment_colour / (environment_colour + vec3(1.0));
	environment_colour = pow(environment_colour, vec3(1.0 / 2.2));

	fragment_colour = vec4(environment_colour, 1.0);
}
