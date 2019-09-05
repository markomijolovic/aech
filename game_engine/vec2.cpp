#include "vec2.hpp"

namespace aech::math
{
	vec2_t::vec2_t(float x, float y)
		: x{ x }, y{ y }
	{
	}

	vec2_t& vec2_t::operator+=(const vec2_t& vec2)
	{
		x += vec2.x;
		y += vec2.y;
		return *this;
	}

	vec2_t& vec2_t::operator-=(const vec2_t& vec2)
	{
		x -= vec2.x;
		y -= vec2.y;
		return *this;
	}

	vec2_t operator+(vec2_t lhs, const vec2_t& rhs)
	{
		return lhs += rhs;
	}

	vec2_t operator-(vec2_t lhs, const vec2_t& rhs)
	{
		return lhs -= rhs;
	}
}
