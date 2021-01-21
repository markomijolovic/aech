#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

out vec3 world_position;

void main()
{
    world_position = position;

    vec4 clip_position = projection * mat4(mat3(view)) * vec4(world_position, 1.0);

    gl_Position = clip_position.xyww;
}
