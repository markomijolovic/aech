#version 460 core

in vec2 uvs;

out vec4 fragment_colour;

uniform sampler2D texture_position;
uniform sampler2D texture_normal;
uniform sampler2D texture_albedo;
uniform sampler2D texture_metallic_roughness_ao;
uniform sampler2DShadow light_shadow_map;

uniform vec3 light_dir;
uniform vec3 light_colour;
uniform float light_intensity;
uniform mat4 depth_bias_vp;

uniform float poisson_sampling_distance_multiplier;
uniform bool shadows;
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

const vec2 poisson_disk[16] = vec2[]
( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

float random(vec3 seed, int i) 
{
	vec4 seed4 = vec4(seed, i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

float shadow(vec3 position, float cosangle) {
	vec4 shadow_coords = depth_bias_vp * vec4(position, 1.0);

	float visibility = 1.0;
	float bias = clamp(0.005*tan(acos(cosangle)), 0.0, 0.01);
	vec2 texel_size = 1.0 / textureSize(light_shadow_map, 0);

	// sample the shadow map 16 times
	for (int i = 0; i < 4; i++) 
	{
		int index = int(16.0*random(floor(position * 1000.0), i)) %16;
		visibility -= 0.25*(1.0-texture( light_shadow_map, vec3(shadow_coords.xy + poisson_disk[index]*poisson_sampling_distance_multiplier*texel_size,  shadow_coords.z-bias )));
	}

	return visibility;
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
	vec3 diffuse = mix((vec3(1.0) - f), vec3(0), metallic) * (albedo / pi); 

	vec3 outgoing_radiance = (diffuse + specular) * radiance * max(dot(normal, light), 0.0);
	if (shadows) outgoing_radiance *= shadow(position, max(dot(normal, light), 0.0));

	fragment_colour = vec4(outgoing_radiance, 1.0);
}
