#version 460 core

layout(location = 0) in vec3 position;

out vec3 world_position;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    world_position = position;

    gl_Position = projection * view * vec4(position, 1.0);
}
