#version 450 core

out vec4 fragment_colour;

void main()
{
	fragment_colour = vec4(vec3(gl_FragCoord.z), 1.0);
}
