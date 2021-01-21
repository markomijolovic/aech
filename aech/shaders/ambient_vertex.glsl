#version 460 core

layout(location = 0) in vec3 position;

out vec3 fragment_position;
out vec4 screen_position;
out vec3 pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    fragment_position = vec3(model * vec4(position, 1.0));

    screen_position = projection * view * vec4(fragment_position, 1.0);
    pos = position;
    gl_Position = screen_position;
}
