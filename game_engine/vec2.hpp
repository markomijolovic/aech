#pragma once

namespace aech
{
	class vec2_t
	{
	public:
		float x, y;

		vec2_t() = default;
		vec2_t(float x, float y);

		vec2_t& operator+=(const vec2_t& vec2);
		vec2_t& operator-=(const vec2_t& vec2);
	};

	vec2_t operator+(vec2_t lhs, const vec2_t& rhs);
	vec2_t operator-(vec2_t lhs, const vec2_t& rhs);
}