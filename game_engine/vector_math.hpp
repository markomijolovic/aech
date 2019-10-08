#pragma once
#include "vec4.hpp"

namespace aech::math
{
	inline auto dot (const vec4_t& a, const vec4_t& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	};

	inline auto distance_squared(const vec3_t& a, const vec3_t& b)
	{
		return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
	}
}
