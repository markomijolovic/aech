#version 450 core

out vec4 fragment_colour;

in vec2 in_uvs;
in vec3 in_normal;
in vec3 in_fragment_world_position;

uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D metallic_map;
uniform sampler2D roughness_map;
uniform sampler2D ambient_occlusion_map;

uniform vec3 point_light_positions[10];
uniform vec3 point_light_intensities[10];

uniform vec3 camera_position;

const float pi = 3.14159265359;

vec3 get_normal() 
{
	return vec3(1.0);
}

float ggx_distribution(vec3 normal, vec3 halfway, float roughness) 
{
	return pow(roughness, 2.0) / (pi*pow(pow(dot(normal, halfway), 2.0) * (pow(roughness, 2.0) - 1) + 1,2.0));
}

float schlick_ggx_helper(float ndotv, float k) 
{
	return ndotv / (ndotv*(1.0-k) + k);
}

// this is not physically based, move to pure smith's function
float smith_masking(vec3 normal, vec3 view, vec3 light, float k)
{
	float ndotv = max(dot(normal, view), 0.0);
	float ndotl = max(dot(normal, light), 0.0);

	float ggx_view = schlick_ggx_helper(ndotv, k);
	float ggx_light = schlick_ggx_helper(ndotl,  k);

	return ggx_view * ggx_light;
}

// schlick's approximation of the fresnel factor
vec3 schlicks_approximation(float cos_angle, vec3 f0) 
{
	return f0 + (1.0-f0) * pow(1-cos_angle, 5.0);
}

void main()
{
	vec3 albedo = pow(texture(albedo_map, in_uvs).rgb, vec3(2.2));
	float metallic = texture(metallic_map, in_uvs).r;
	float roughness = texture(roughness_map, in_uvs).r;
	float ao = texture(ambient_occlusion_map, in_uvs).r;

	vec3 normal = get_normal();
	vec3 view = normalize(camera_position - in_fragment_world_position);

	vec3 f0 = mix(vec3(0.04), albedo, metallic);

	vec3 outgoing_radiance = vec3(0.0);
	for (int i = 0; i < 10; i++) 
	{
		vec3 light = normalize(point_light_positions[i] - in_fragment_world_position);
		vec3 halfway = normalize(view + light);
		
		float distance = length(point_light_positions[i] - in_fragment_world_position);
		float attenuation = 1.0/pow(distance, 2.0);
		vec3 radiance = point_light_intensities[i] * attenuation;

		// cook-torrance brdf
		float ndf = ggx_distribution(normal, halfway, roughness);
		float smith = smith_masking(normal, view, light, roughness);
		vec3 f = schlicks_approximation(max(dot(halfway, view), 0.0), f0);
		
		vec3 specular = (ndf * smith * f)/(4.0 * max(dot(normal, view), 0.0) * max(dot(normal, light), 0.0) + 0.00001);
	
		vec3 diffuse = vec3(1.0 - f) * (1.0 - metallic);

		outgoing_radiance += (diffuse * albedo/pi + specular) * radiance * max(dot(normal, light), 0.0);
	}

	vec3 ambient = vec3(0.03) * albedo * ao;

	vec3 color = ambient + outgoing_radiance;

	color = color/(color+ vec3(1.0));

	color = pow(color, vec3(1.0/2.2));

	fragment_colour = vec4(color, 1.0);
}