#pragma once
#include "vec4.hpp"

namespace aech::math
{
	inline float dot (const vec4_t& a, const vec4_t& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	};
}
