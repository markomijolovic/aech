#version 460 core

out vec4 fragment_colour;

in vec2 uvs;

uniform sampler2D texture_albedo;

void main()
{
    if (texture(texture_albedo, uvs).a < 0.5)
    {
        discard;
    }
    fragment_colour = vec4(vec3(gl_FragCoord.z), 1.0);
}
