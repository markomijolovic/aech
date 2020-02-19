#version 460 core

out vec4 fragment_colour;

in vec3 world_position;

uniform samplerCube environment_map;
uniform float roughness;

const float pi = 3.14159265358979;

float radical_inverse_vdc(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10;
}

vec2 hammersley2d(uint i, uint N)
{
	return vec2(float(i) / float(N), radical_inverse_vdc(i));
}

vec3 importance_sample_ggx(vec2 x_i, vec3 n, float roughness)
{
	float a = roughness * roughness;

	float phi = 2 * pi * x_i.x;
	float costheta = sqrt((1 - x_i.y) / (1 + (a * a - 1) * x_i.y));
	float sintheta = sqrt(1 - costheta * costheta);

	vec3 h = vec3(cos(phi) * sintheta, sin(phi) * sintheta, costheta);

	vec3 up = abs(n.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent = normalize(cross(up, n));
	vec3 bitangent = cross(n, tangent);

	vec3 sample_vec = tangent * h.x + bitangent * h.y + n * h.z;
	return normalize(sample_vec);
}


void main()
{
	vec3 n = normalize(world_position);

	vec3 r = n;
	vec3 v = r;

	const uint sample_count = 1024;
	float total_weight = 0;
	vec3 prefiltered_colour = vec3(0.0);

	for (uint i = 0; i < sample_count; i++)
	{
		vec2 x_i = hammersley2d(i, sample_count);
		vec3 h = importance_sample_ggx(x_i, n, roughness);
		vec3 l = normalize(2 * dot(v, h) * h - v);

		float ndotl = max(dot(n, l), 0.0);
		if (ndotl > 0.0) {
			prefiltered_colour += texture(environment_map, l).rgb * ndotl;
			total_weight += ndotl;
		}
	}

	prefiltered_colour = prefiltered_colour / total_weight;
	fragment_colour = vec4(prefiltered_colour, 1.0);
}
