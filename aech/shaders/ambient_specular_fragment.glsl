#version 460 core

out vec4 fragment_colour;

in vec3 fragment_position;
in vec4 screen_position;

uniform sampler2D texture_position;
uniform sampler2D texture_normal;
uniform sampler2D texture_albedo;
uniform sampler2D texture_metallic_roughness_ao;
uniform sampler2D texture_ssao;

uniform bool ssao;

uniform samplerCube environment_prefiltered;
uniform sampler2D brdf_lut;

uniform vec3 camera_position;
uniform vec3 probe_position;
uniform vec4 box_min;
uniform vec4 box_max;

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

void main()
{
    vec2 uv = (screen_position.xy / screen_position.w) * 0.5 + 0.5;

    vec3 albedo = texture(texture_albedo, uv).rgb;
    vec3 normal = texture(texture_normal, uv).rgb;
    vec3 world_position = texture(texture_position, uv).rgb;
    vec3 mra = texture(texture_metallic_roughness_ao, uv).rgb;

    float metallic = mra.r;
    float roughness = mra.g;

    normal = normalize(normal);
    vec3 view = normalize(camera_position - world_position);
    vec3 light = normalize(probe_position - world_position);
    vec3 halfway = normalize(light + view);
    vec3 reflected = reflect(-view, normal);

    // TODO: discard when occluded?

    if (!(box_min.x <= world_position.x && box_min.y <= world_position.y && box_min.z <= world_position.z && box_max.x >= world_position.x && box_max.y >= world_position.y && box_max.z >= world_position.z))
    {
        discard;
    }

    vec3 first_plane_intersect = (vec3(box_max) - world_position) / reflected;
    vec3 second_plane_intersect = (vec3(box_min) - world_position) / reflected;

    vec3 furthest = max(first_plane_intersect, second_plane_intersect);

    float distance = min(min(furthest.x, furthest.y), furthest.z);

    vec3 position = world_position + reflected * distance;

    vec3 sample_vec = position - probe_position;

    //float attenuation = pow(max(1.0 - max(0.0, (length(world_position - probe_position) - inner_radius) / (outer_radius - inner_radius)), 0.0), 2.0);

    vec3 f0 = mix(vec3(0.04), albedo, metallic);
    vec3 f = schlicks_approximation(max(dot(halfway, view), 0.0), f0);

    const float max_lod = 5;
    vec3 prefiltered = textureLod(environment_prefiltered, sample_vec, roughness * max_lod).rgb;
    vec2 brdf = texture(brdf_lut, vec2(max(dot(normal, view), 0.0), roughness)).rg;
    vec3 specular = prefiltered * (f * brdf.x + brdf.y);

    //vec3 colour = attenuation * specular;
    vec3 colour = 0.33*specular;
    // TODO: specular occlusion
    if (ssao) colour *= texture(texture_ssao, uv).r;
    fragment_colour = vec4(colour, 1.0);
}