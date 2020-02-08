#pragma once

#include "vec4.hpp"
#include <initializer_list>
#include <array>

namespace aech::math
{
	class mat4_t
	{
	public:
		mat4_t() = default;
		mat4_t(const std::initializer_list<float>& list);
		mat4_t(const std::initializer_list<vec4_t>& list);

		mat4_t&      operator*=(const mat4_t& rhs);
		mat4_t       operator*(const mat4_t& rhs) const;
		vec4_t       operator*(const vec4_t& rhs) const;
		mat4_t       operator*(float rhs) const;
		const vec4_t& operator[](size_t i) const;
		vec4_t&       operator[](size_t i);

	private:
		// TODO(Marko): maybe use an array of vec4_ts as representation? 
		// TODO(Marko): have to think about it.

		/* float data[4][4] = {
			{1.0F, 0.0F, 0.0F, 0.0F},
			{0.0F, 1.0F, 0.0F, 0.0F},
			{0.0F, 0.0F, 1.0F, 0.0F},
			{0.0F, 0.0F, 0.0F, 1.0F}
		};*/

		std::array<vec4_t, 4> data
		{
		vec4_t{1.0F, 0.0F, 0.0F, 0.0F},
		vec4_t{0.0F, 1.0F, 0.0F, 0.0F},
		vec4_t{0.0F, 0.0F, 1.0F, 0.0F},
		vec4_t{0.0F, 0.0F, 0.0F, 1.0F}
		};
		
	};

	mat4_t operator*(float lhs, const mat4_t& rhs);
} // namespace aech::math