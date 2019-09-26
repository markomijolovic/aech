#version 450 core

out float fragment_colour;

in vec2 uvs;

uniform sampler2D ssao_input;

void main()
{
	const vec2 texel_size = 1.0F / vec2(textureSize(ssao_input, 0));
	float result = 0.0F;

	for (int x = -2; x < 2; x++)
		for (int y = -2; y < 2; y++)
			result += texture(ssao_input, uvs + vec2(x + 0.5F, y + 0.5F)* texel_size).r;

	fragment_colour = result / (16.0F);
}
