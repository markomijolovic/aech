#pragma once
#include "vec4.hpp"

namespace aech::math
{
	// returns the dot product of the given two vectors
	inline auto dot(const vec4_t& a, const vec4_t& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	};

	// returns the squared distance between the given two vectors
	inline auto distance_squared(const vec3_t& a, const vec3_t& b)
	{
		return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
	}
}
