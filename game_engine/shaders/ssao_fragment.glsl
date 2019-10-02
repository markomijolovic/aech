#version 450 core

out float fragment_colour;

in vec2 uvs;
	
uniform sampler2D texture_position;
uniform sampler2D texture_normal;
uniform sampler2D texture_noise;

uniform vec3 samples[64];
uniform vec2 resolution;
uniform mat4 view;
uniform mat4 projection;
uniform float radius;

const vec2 noise_scale = resolution/4.0F;

void main()
{
	vec3 view_pos = texture(texture_position, uvs).xyz;
	view_pos = vec3(view * vec4(view_pos, 1.0));
	vec3 normal = texture(texture_normal, uvs).xyz;
	normal = normalize(vec3(inverse(transpose(view)) * vec4(normal, 1.0)));
	vec3 random_vec = texture(texture_noise, uvs * noise_scale).xyz;

	vec3 tangent = normalize(random_vec - normal * dot(random_vec, normal));
	vec3 bitangent = cross(normal, tangent);

	// tbn transforms from tangent space 
	// to view space
	mat3 tbn = mat3(tangent, bitangent, normal);

	float occlusion = 0.0F;
	// 32 == sample_count
	for (int i = 0; i < 32; i++) 
	{
		// _sample position in view space
		vec3 _sample = tbn * samples[i];
		_sample = view_pos + _sample*radius; // _sample position in view space
		
		vec4 offset = vec4(_sample, 1.0F);
		offset = projection * offset;
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5; // _sample position in screen space

		float _sample_depth = vec3(view * vec4(texture(texture_position, offset.xy).xyz, 1.0)).z; // depth in view space


		float range_check = smoothstep(0.0F, 1.0F, radius/ abs(view_pos.z - _sample_depth));
		// maybe add bias?
		occlusion += (_sample_depth >= _sample.z ? 1.0F:0.0F) * range_check;
	}

	occlusion = 1.0F - (occlusion/32);
	fragment_colour = occlusion;
}
