#pragma once

#include "vec4.hpp"
#include <initializer_list>

namespace aech::math
{
	class mat4_t
	{
	public:
		mat4_t() = default;
		mat4_t(const std::initializer_list<float>& list);
		mat4_t(const std::initializer_list<vec4_t>& list);

		mat4_t& operator*=(const mat4_t& rhs);
		mat4_t operator*(const mat4_t& rhs) const;
		vec4_t operator*(const vec4_t& rhs) const;
		mat4_t operator*(float rhs) const;
		const float* operator[](size_t i) const;
		float* operator[](size_t i);

	private:
		// TODO: maybe use an array of vec4_ts as representation? 
		// TODO: have to think about it.
		float data[4][4] = {
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};
	};

	mat4_t operator*(float lhs, const mat4_t& rhs);
}
