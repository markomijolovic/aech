#version 450 core

out vec4 fragment_colour;

in vec2 uvs;
in vec3 fragment_position;
in vec3 normal;

uniform sampler2D texture_albedo;
uniform sampler2D texture_normal;
uniform sampler2D texture_metallic;
uniform sampler2D texture_roughness;

uniform vec3 light_dir;
uniform vec3 light_colour;
uniform float light_intensity;

void main()
{
	vec4 albedo = texture(texture_albedo, uvs);
	vec3 n = normalize(normal);
	float metallic = texture(texture_metallic, uvs).r;
	float roughness = texture(texture_roughness, uvs).r;
	//for now
	if (albedo.a < 0.5) discard;
	fragment_colour = albedo;
}
