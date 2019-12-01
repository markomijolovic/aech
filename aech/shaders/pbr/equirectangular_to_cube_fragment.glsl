#version 450 core

in vec3 world_position;

out vec4 fragment_colour;

uniform sampler2D equirectangular_map;

const float pi = 3.1415926535897932384626433832795;

vec2 get_uvs(vec3 v) 
{
	return vec2(atan(v.z, v.x), asin(v.y)) * vec2(1/(2*pi), 1/pi) + 0.5;
}

void main()
{
	vec2 uv = get_uvs(normalize(world_position));	
	vec3 colour = texture(equirectangular_map, uv).rgb;

	fragment_colour = vec4(colour, 1.0);
}
