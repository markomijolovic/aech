#include "vector_math.hpp"


float aech::math::dot(const vec4_t& a, const vec4_t& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
