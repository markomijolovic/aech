#version 450 core

out vec4 fragment_colour;

in vec3 fragment_position;
in vec4 screen_position;
in vec3 camera_position;
in vec3 probe_position;

void main()
{
	vec2 uv = (screen_position.xy / screen_position.w) * 0.5 + 0.5;


}
