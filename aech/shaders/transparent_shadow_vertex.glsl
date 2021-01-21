#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 0) in vec2 in_uvs;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 uvs;

void main()
{
    uvs = in_uvs;
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
