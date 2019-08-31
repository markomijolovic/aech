#version 450 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_uvs;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec3 in_tangent;
layout (location = 4) in vec3 in_bitangent;

out VERTEX_OUT 
{
	vec3 fragment_world_position;
	vec2 uvs;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
} vertex_out;

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

	vertex_out.fragment_world_position = vec3(model * vec4(in_position, 1.0));
	vertex_out.normal = n;
	vertex_out.uvs = in_uvs;
	vertex_out.tangent = t;
	vertex_out.bitangent = b;
	gl_Position = projection * view * model * vec4(in_position, 1.0);
}
