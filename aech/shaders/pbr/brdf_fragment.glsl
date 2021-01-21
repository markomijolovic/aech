#version 460 core

out vec2 fragment_colour;

in vec2 uvs;

const float pi = 3.14159265359;

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

vec2 integrate_brdf(float ndotv, float roughness)
{
    vec3 v = vec3(sqrt(1 - ndotv * ndotv), 0, ndotv);

    float a = 0;
    float b = 0;
    vec3 n = vec3(0, 0, 1);

    const uint sample_count = 1024;
    for (uint i = 0; i < sample_count; i++)
    {
        vec2 x_i = hammersley2d(i, sample_count);
        vec3 h = importance_sample_ggx(x_i, n, roughness);
        vec3 l = normalize(2 * dot(v, h) * h - v);

        float ndotl = max(l.z, 0.0);
        float ndoth = max(h.z, 0.0);
        float vdoth = max(dot(v, h), 0.0);

        if (ndotl > 0.0)
        {
            float g = smith_masking(n, v, l, (roughness * roughness) / 2);
            float gvis = (g * vdoth) / (ndoth * ndotv);
            float fc = pow(1 - vdoth, 5);
            a += (1 - fc) * gvis;
            b += fc * gvis;
        }
    }
    a /= sample_count;
    b /= sample_count;
    return vec2(a, b);
}

void main()
{
    fragment_colour = integrate_brdf(uvs.x, uvs.y);
}
