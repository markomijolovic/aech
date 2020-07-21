#pragma once
#include "aech_types.hpp"

#include "mat.hpp"

#include "transform.hpp"

#include "vec.hpp"

namespace aech::math
{
	// computes the angle in radians from the provided angle which is assumed to be in degrees
	inline auto radians(float angle)
	{
		return angle * (pi / 180.0F);
	}

	// linearly interpolates between a and b
	inline auto lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	// returns the translation matrix for the given x, y and z translation values
	mat4_t translate(float x, float y, float z);

	// returns the translation matrix for the given x, y and z translation values
	mat4_t translate(const vec3_t& vec3);

	// returns the scale matrix for the given x, y and z scale values
	mat4_t scale(float x, float y, float z);

	// returns the scale matrix for the given x, y and z scale values
	mat4_t scale(const vec3_t& vec3);

	// returns the scale matrix for the given (same) x, y and z scale values
	mat4_t scale(float x);

	// returns the rotation matrix for the given angle and axis
	mat4_t rotate(float angle, const vec3_t& axis);

	// returns the normalized vector
	vec3_t normalize(const vec3_t& axis);

	// returns the view matrix
	mat4_t get_view_matrix(const transform_t& transform);

	// returns the cross product of the given two vectors
	vec3_t cross(const vec3_t& first, const vec3_t& second);

	// returns the look at matrix
	mat4_t look_at(const vec3_t& eye, const vec3_t& centre, const vec3_t& up);

	// returns the perspective matrix
	mat4_t perspective(float fov_y, float aspect, float z_near, float z_far);

	// returns the orthographic matrix
	mat4_t orthographic(float left, float right, float bottom, float top, float near, float far);
}

// namespace aech::math
