#pragma once

#include "vec4.hpp"
#include <initializer_list>
#include <array>

namespace aech::math
{
	// a class that represents a four by four matrix
	class mat4_t
	{
	public:
		// identity matrix
		mat4_t() = default;

		// initializes the matrix from the given init list of floats
		mat4_t(const std::initializer_list<float>& list);

		// initializes the matrix from the given init list of four component vectors
		mat4_t(const std::initializer_list<vec4_t>& list);

		mat4_t& operator*=(const mat4_t& rhs);
		mat4_t operator*(const mat4_t& rhs) const;
		vec4_t operator*(const vec4_t& rhs) const;
		mat4_t operator*(float rhs) const;
		const vec4_t& operator[](size_t i) const;
		vec4_t& operator[](size_t i);

	private:
		// todo(Marko): maybe use an array of vec4_ts as representation? 
		// todo(Marko): have to think about it.

		/* float data[4][4] = {
			{1.0F, 0.0F, 0.0F, 0.0F},
			{0.0F, 1.0F, 0.0F, 0.0F},
			{0.0F, 0.0F, 1.0F, 0.0F},
			{0.0F, 0.0F, 0.0F, 1.0F}
		};*/

		std::array<vec4_t, 4> m_data
		{
			vec4_t{1.0F, 0.0F, 0.0F, 0.0F},
			vec4_t{0.0F, 1.0F, 0.0F, 0.0F},
			vec4_t{0.0F, 0.0F, 1.0F, 0.0F},
			vec4_t{0.0F, 0.0F, 0.0F, 1.0F}
		};
	};

	mat4_t operator*(float lhs, const mat4_t& rhs);
} // namespace aech::math
