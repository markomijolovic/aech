#version 460 core

layout(location = 0) in vec3 position;

out vec3 fragment_position;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    fragment_position = position;
    vec4 clip_position = projection * mat4(mat3(view)) * vec4(position, 1.0);

    gl_Position = clip_position.xyww;
}
