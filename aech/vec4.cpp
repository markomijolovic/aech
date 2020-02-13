#include "vec4.hpp"
#include "vec3.hpp"
#include <stdexcept>

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

	vec4_t& vec4_t::operator-=(const vec4_t& vec4)
	{
		x -= vec4.x;
		y -= vec4.y;
		z -= vec4.z;
		w -= vec4.w;
		return *this;
	}


	vec4_t& vec4_t::operator*=(float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}


	vec4_t operator*(vec4_t lhs, float rhs)
	{
		return lhs *= rhs;
	}

	vec4_t operator*(float lhs, vec4_t rhs)
	{
		return rhs *= lhs;
	}


	vec4_t& vec4_t::operator/=(float rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		return *this;
	}

	vec4_t operator/(vec4_t lhs, float rhs)
	{
		return lhs /= rhs;
	}

	vec4_t operator/(float lhs, vec4_t rhs)
	{
		return rhs /= lhs;
	}

	vec4_t operator+(vec4_t lhs, const vec4_t& rhs)
	{
		return lhs += rhs;
	}

	vec4_t operator-(vec4_t lhs, const vec4_t& rhs)
	{
		return lhs -= rhs;
	}

	float& vec4_t::operator[](size_t index)
	{
		switch (index)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			default:
				throw std::runtime_error{"Index out of bounds for vec4_t's operator[]"};
		}
	}

	const float& vec4_t::operator[](size_t index) const
	{
		switch (index)
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			default:
				throw std::runtime_error{"Index out of bounds for vec4_t's operator[]"};
		}
	}
} // namespace aech::math
