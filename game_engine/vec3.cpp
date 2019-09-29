#include "vec3.hpp"
#include "vec4.hpp"
#include <stdexcept>

namespace aech::math
{
	vec3_t::vec3_t(float x, float y, float z) :
		x{x},
		y{y},
		z{z}
	{
	}


	vec3_t::vec3_t(const vec4_t& vec4) :
		x{vec4.x},
		y{vec4.y},
		z{vec4.z}
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

	vec3_t& vec3_t::operator*=(float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	float& vec3_t::operator[](size_t index)
	{
		switch(index)
		{
		case 0: return x; 
		case 1: return y; 
		case 2: return z; 
		default: throw std::runtime_error {"Index out of bounds for vec3_t's operator[]"};
		}
	}

	const float& vec3_t::operator[](size_t index) const
	{
		switch(index)
		{
		case 0: return x; 
		case 1: return y; 
		case 2: return z; 
		default: throw std::runtime_error {"Index out of bounds for vec3_t's operator[]"};
		}
	}

	vec3_t operator*(vec3_t lhs, float rhs)
	{
		return lhs *= rhs;
	}

	


	vec3_t operator*(float lhs, vec3_t rhs)
	{
		return rhs *= lhs;
	}
} // namespace aech::math
