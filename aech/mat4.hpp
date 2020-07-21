#pragma once

#include <initializer_list>

#include "vec.hpp"

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
		[[nodiscard]] mat4_t operator*(const mat4_t& rhs) const;
		[[nodiscard]] vec4_t operator*(const vec4_t& rhs) const;
		[[nodiscard]] mat4_t operator*(float rhs) const;
		const vec4_t& operator[](size_t i) const;
		vec4_t& operator[](size_t i);
		
	private:
		std::array<vec4_t, 4> m_data
		{ 

			{{1.0F, 0.0F, 0.0F, 0.0F},
			{0.0F, 1.0F, 0.0F, 0.0F},
			{0.0F, 0.0F, 1.0F, 0.0F},
			{0.0F, 0.0F, 0.0F, 1.0F}}
		};

	};

	mat4_t operator*(float lhs, const mat4_t& rhs);
} // namespace aech::math
