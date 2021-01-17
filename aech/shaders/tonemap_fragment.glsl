#version 460 core

out vec4 fragment_colour;

in vec2 uvs;

uniform sampler2D tex;

vec3 aces(const vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d ) + e), 0.0, 1.0);
}


void main()
{
	vec3 colour = texture(tex, uvs).rgb;
	//colour = aces(colour);
	colour = colour/(vec3(1)+colour);

	// convert linear to sRGB

	if (colour.r > 0.0031308)
	{
		colour.r = 1.055 * pow(colour.r, 1/2.4) - 0.055;
	}
	else
	{
		colour.r = 12.92 * colour.r;
	}

	if (colour.g > 0.0031308)
	{
		colour.g = 1.055 * pow(colour.g, 1/2.4) - 0.055;
	}
	else
	{
		colour.g = 12.92 * colour.g;
	}

	if (colour.b > 0.0031308)
	{
		colour.b = 1.055 * pow(colour.b, 1/2.4) - 0.055;
	}
	else
	{
		colour.b = 12.92 * colour.b;
	}

	fragment_colour = vec4(colour, 1.0);
}
