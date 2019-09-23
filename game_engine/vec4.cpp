#include "vec4.hpp"
#include "vec3.hpp"

namespace aech::math
{
	vec4_t::vec4_t(const vec3_t& vec3, float w) :
		x{vec3.x},
		y{vec3.y},
		z{vec3.z},
		w{w}
	{
	}

	vec4_t::vec4_t(float x, float y, float z, float w) :
		x{x},
		y{y},
		z{z},
		w{w}
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


	vec4_t& vec4_t::operator*(float f)
	{
		x*=f;
		y*=f;
		z*=f;
		w*=f;
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


	float dot(const vec4_t& lhs, const vec4_t& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}
} // namespace aech::math
