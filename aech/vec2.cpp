#include "vec2.hpp"
#include <stdexcept>

namespace aech::math
{
	vec2_t::vec2_t(float x, float y) :
		x{x},
		y{y}
	{
	}

	vec2_t& vec2_t::operator+=(const vec2_t& vec2)
	{
		x += vec2.x;
		y += vec2.y;
		return *this;
	}


	float& vec2_t::operator[](size_t index)
	{
		switch (index)
		{
			case 0:
				return x;
			case 1:
				return y;
			default:
				throw std::runtime_error{"Index out of bound on vec2_t's operator[]"};
		}
	}

	const float& vec2_t::operator[](size_t index) const
	{
		switch (index)
		{
			case 0:
				return x;
			case 1:
				return y;
			default:
				throw std::runtime_error{"Index out of bound on vec2_t's operator[]"};
		}
	}

	vec2_t& vec2_t::operator-=(const vec2_t& vec2)
	{
		x -= vec2.x;
		y -= vec2.y;
		return *this;
	}


	vec2_t& vec2_t::operator*=(float rhs)
	{
		x *= rhs;
		y *= rhs;
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


	vec2_t operator*(vec2_t lhs, float rhs)
	{
		return lhs *= rhs;
	}

	vec2_t operator*(float lhs, vec2_t rhs)
	{
		return rhs *= lhs;
	}
} // namespace aech::math
