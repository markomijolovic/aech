#version 460 core

out vec4 fragment_colour;

in vec3 fragment_position;

uniform samplerCube skybox;

void main()
{
    vec3 albedo = texture(skybox, fragment_position).rgb;

    fragment_colour = vec4(albedo, 1.0);
}
