#include "vec4.hpp"

namespace aech
{
	vec4_t::vec4_t(float x, float y, float z, float w)
		: x{ x }, y{ y }, z{ z }, w{ w }
	{
	}

	vec4_t& vec4_t::operator+=(const vec4_t& vec4)
	{
		x += vec4.x;
		y += vec4.y;
		z += vec4.z;
		w += vec4.w;
		return *this;
	}

	vec4_t& vec4_t::operator-=(const vec4_t& vec4)
	{
		x -= vec4.x;
		y -= vec4.y;
		z -= vec4.z;
		w -= vec4.w;
		return *this;
	}

	vec4_t operator+(vec4_t lhs, const vec4_t& rhs)
	{
		return lhs += rhs;
	}

	vec4_t operator-(vec4_t lhs, const vec4_t& rhs)
	{
		return lhs -= rhs;
	}
}
