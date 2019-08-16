#pragma once

#include "vec4.hpp"

namespace aech
{
	class mat4_t
	{
	public:
		float data[4][4] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		mat4_t operator*(const mat4_t& rhs);
		vec4_t operator*(const vec4_t& rhs);
	};
}