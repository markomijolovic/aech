#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uvs;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in vec3 in_bitangent;

out vec3 fragment_world_position;
out	vec2 uvs;
out	vec3 normal;
out	vec3 tangent;
out	vec3 bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat3 normal_matrix = transpose(inverse(mat3(model)));
	vec3 t = normalize(normal_matrix * in_tangent);
	vec3 n = normalize(normal_matrix * in_normal);
	t = normalize(t - dot(t, n) * n);
	vec3 b = cross(n, t);

	fragment_world_position = vec3(model * vec4(in_position, 1.0));
	uvs = in_uvs;
	tangent = t;
	bitangent = b;
	normal = n;

	gl_Position = projection * view * model * vec4(in_position, 1.0);
}
