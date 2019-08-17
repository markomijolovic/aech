#pragma once
#include "mat4.hpp"
#include "vec3.hpp"
#include "types.hpp"
#include "components.hpp"

namespace aech
{

	inline auto radians(float angle)
	{
		return (angle * pi) / 180.0f;
	}

	mat4_t translate(float x, float y, float z);
	mat4_t scale(float x, float y, float z);
	mat4_t rotate(float angle, const vec3_t& axis);
	vec3_t normalize(const vec3_t& axis);
	mat4_t get_view_matrix(const transform_t& transform);
}
