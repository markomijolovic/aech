#version 450 core

in vec2 uvs;

out vec4 fragment_colour;

uniform sampler2D texture_position;
uniform sampler2D texture_normal;
uniform sampler2D texture_albedo;
uniform sampler2D texture_metallic_roughness_ao;
uniform sampler2D light_shadow_map;

uniform vec3 light_dir;
uniform vec3 light_colour;
uniform float light_intensity;
uniform mat4 depth_bias_vp;

uniform vec3 camera_position;

const float pi = 3.14159265359;

float ggx_distribution(vec3 normal, vec3 halfway, float alpha)
{
	return pow(alpha, 2) / (pi * pow(pow(dot(normal, halfway), 2) * (pow(alpha, 2) - 1) + 1, 2));
}

float schlick_ggx_helper(float ndotv, float k)
{
	return ndotv / (ndotv * (1.0 - k) + k);
}

float smith_masking(vec3 normal, vec3 view, vec3 light, float k)
{
	float ndotv = max(dot(normal, view), 0.0);
	float ndotl = max(dot(normal, light), 0.0);

	float ggx_view = schlick_ggx_helper(ndotv, k);
	float ggx_light = schlick_ggx_helper(ndotl, k);

	return ggx_view * ggx_light;
}

vec3 schlicks_approximation(float cos_angle, vec3 f0)
{
	return f0 + (1.0 - f0) * pow(1 - cos_angle, 5.0);
}

// TODO: sampling
float shadow(vec3 position, float cosangle) {
	vec4 shadow_coords = depth_bias_vp * vec4(position, 1.0);
	float bias = max(0.05 * cosangle, 0.005);
	if (texture(light_shadow_map, shadow_coords.xy).r < shadow_coords.z - bias ) {
		return 0;
	}
	return 1;
}

void main()
{
	vec3 position = texture(texture_position, uvs).rgb;
	vec3 albedo = texture(texture_albedo, uvs).rgb;
	vec3 normal = texture(texture_normal, uvs).rgb;

	vec3 m_r_ao = texture(texture_metallic_roughness_ao, uvs).rgb;
	float metallic = m_r_ao.r;
	float roughness = m_r_ao.g;

	normal = normalize(normal);
	vec3 view = normalize(camera_position - position);
	vec3 light = normalize(-light_dir);
	vec3 halfway = normalize(view + normal);
	vec3 f0 = mix(vec3(0.04), albedo, metallic);

	vec3 radiance = light_colour * light_intensity;
	// cook-torrance brdf
	float ndf = ggx_distribution(normal, halfway, pow(roughness, 2));
	float smith = smith_masking(normal, view, light, pow((roughness + 1) / 2, 2));
	vec3 f = schlicks_approximation(max(dot(halfway, view), 0.0), f0);
	vec3 specular = (ndf * smith * f) / (4.0 * max(dot(normal, view), 0.0) * max(dot(normal, light), 0.0) + 0.00001);
	vec3 diffuse = (vec3(1.0 - f) * (1.0 - metallic)) * (albedo / pi);

	// TODO: add shadows
	vec3 outgoing_radiance = (diffuse + specular) * radiance * max(dot(normal, light), 0.0) * shadow(position, max(dot(normal, light), 0.0));

	fragment_colour = vec4(albedo * 0.25 + outgoing_radiance, 1.0);
}
