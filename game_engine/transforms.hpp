#pragma once
#include "mat4.hpp"
#include "vec3.hpp"
#include "types.hpp"

namespace aech
{
	struct transform_t;

	inline auto radians(float angle)
	{
		return (angle * pi) / 180.0f;
	}

	mat4_t translate(float x, float y, float z);
	mat4_t translate(const vec3_t& vec3);
	mat4_t scale(float x, float y, float z);
	mat4_t scale(const vec3_t& vec3);
	mat4_t rotate(float angle, const vec3_t& axis);
	vec3_t normalize(const vec3_t& axis);
	mat4_t get_view_matrix(const transform_t& transform);
	vec3_t cross(const vec3_t& first, const vec3_t& second);

	mat4_t look_at(const vec3_t& eye, const vec3_t& centre, const vec3_t& up);
	mat4_t perspective(float fov_y, float aspect, float z_near, float z_far);
	mat4_t orthographic(float left, float right, float bottom, float top, float near, float far);
}
