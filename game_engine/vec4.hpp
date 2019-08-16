#pragma once

namespace aech
{
	class vec4_t
	{
	public:
		float x, y, z, w;

		vec4_t() = default;
		vec4_t(float x, float y, float z, float w);

		vec4_t& operator+=(const vec4_t& vec4);
		vec4_t& operator-=(const vec4_t& vec4);
	};

	vec4_t operator+(vec4_t lhs, const vec4_t& rhs);
	vec4_t operator-(vec4_t lhs, const vec4_t& rhs);
}