#include "vec3.hpp"

namespace aech
{
	vec3_t::vec3_t(float x, float y, float z)
		:x {x}, y{y}, z{z}
	{
	}

	vec3_t& vec3_t::operator+=(const vec3_t& vec3)
	{
		x += vec3.x;
		y += vec3.y;
		z += vec3.z;
		return *this;
	}

	vec3_t& vec3_t::operator-=(const vec3_t& vec3)
	{
		x -= vec3.x;
		y -= vec3.y;
		z -= vec3.z;
		return *this;
	}

	vec3_t operator+(vec3_t lhs, const vec3_t& rhs)
	{
		return lhs += rhs;
	}

	vec3_t operator-(vec3_t lhs, const vec3_t& rhs)
	{
		return lhs -= rhs;
	}


	vec3_t& vec3_t::operator*=(const vec3_t& vec3)
	{
		x *= vec3.x;
		y *= vec3.y;
		z *= vec3.z;
		return *this;
	}

	vec3_t& vec3_t::operator*=(float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	vec3_t operator*(vec3_t lhs, const vec3_t& rhs)
	{
		return lhs *= rhs;
	}

	vec3_t operator*(vec3_t lhs, float rhs)
	{
		return lhs *= rhs;
	}
}
