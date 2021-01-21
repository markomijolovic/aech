#version 460 core

layout(location = 0) out vec4 position;
layout(location = 1) out vec4 normal_out;
layout(location = 2) out vec4 albedo;
layout(location = 3) out vec4 metallic_roughness_ao;

in vec3 fragment_world_position;
in vec2 uvs;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;

uniform sampler2D texture_albedo;
uniform sampler2D texture_normal;
uniform sampler2D texture_metallic;
uniform sampler2D texture_roughness;

vec3 get_normal()
{
    mat3 tbn = mat3(tangent, bitangent, normal);
    vec3 n = texture(texture_normal, uvs).rgb;
    n = normalize(n * 2.0 - 1.0);
    return normalize(tbn * n);
}

void main()
{
    position = vec4(fragment_world_position, 1.0);
    normal_out = vec4(get_normal(), 1.0);
    albedo = texture(texture_albedo, uvs);
    if (albedo.a < 0.5) discard;
    //albedo = vec4(pow(texture(texture_albedo, uvs).rgb, vec3(2.2)), 1.0);

    metallic_roughness_ao.r = texture(texture_metallic, uvs).r;
    metallic_roughness_ao.g = texture(texture_roughness, uvs).r;
}
