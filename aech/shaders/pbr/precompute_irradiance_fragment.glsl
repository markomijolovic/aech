#version 460 core

in vec3 world_position;

out vec4 fragment_colour;

uniform samplerCube environment_map;

const float pi = 3.14159265359;

void main()
{
    vec3 n = normalize(world_position);

    vec3 up = vec3(0.0, -1.0, 0.0);
    vec3 right = cross(up, n);
    up = cross(n, right);


    vec3 irradiance = vec3(0.0);
    float sample_delta = 0.015;
    int num_samples = 0;
    for (float phi = 0.0; phi < 2 * pi; phi += sample_delta) {
        for (float theta = 0.0; theta < 0.5 * pi; theta += sample_delta) {
            vec3 tangent_sample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 world_sample = tangent_sample.x * right + tangent_sample.y * up + tangent_sample.z * n;

            irradiance += texture(environment_map, world_sample).rgb * cos(theta) * sin(theta);
            num_samples++;
        }
    }
    irradiance = pi * irradiance * (1.0 / num_samples);
    fragment_colour = vec4(irradiance, 1.0);
}
