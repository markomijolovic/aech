#version 450 core

out vec4 FragColor;

in vec2 uvs;
in vec3 normal;
in vec3 fragment_world_position;

uniform vec3 uColor;
uniform sampler2D diffuse;

vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

vec3 lightPos = vec3(-100.0f, 50.0f, 200.0f);

void main()
{
	// Ambient
	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragment_world_position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 difff = diff * lightColor;

	vec4 result = vec4((ambient + difff), 1.0) * texture(diffuse, uvs);
	gl_FragColor = result;
}