#version 450 core

layout(location = 0) out vec4 position_out;
layout(location = 1) out vec4 normal_out;
layout(location = 2) out vec4 albedo;
layout(location = 3) out vec4 metallic_roughness_ao;


in vec3 position;
in vec3 normal;

uniform vec4 colour;

void main()
{
	position_out = vec4(position, 1.0F);
	normal_out = vec4(normalize(normal), 1.0F);
	albedo = colour;
	metallic_roughness_ao = vec4(0.1F, 0.9F, 0.0F, 0.0F);
}
