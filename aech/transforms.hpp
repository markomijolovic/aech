#pragma once

#include "aech_types.hpp"
#include "mat.hpp"
#include "transform.hpp"
#include "vec.hpp"

namespace aech::math {
// computes the angle in radians from the provided angle which is assumed to be in degrees
constexpr auto radians(float angle) noexcept
{
    return angle * (pi / 180.0F);
}

// linearly interpolates between a and b
constexpr auto lerp(float a, float b, float t) noexcept
{
    return a + t * (b - a);
}

// returns the translation matrix for the given x, y and z translation values
auto translate(float x, float y, float z) -> mat4_t;

// returns the translation matrix for the given x, y and z translation values
auto translate(const vec3_t &vec3) -> mat4_t;

// returns the scale matrix for the given x, y and z scale values
auto scale(float x, float y, float z) -> mat4_t;

// returns the scale matrix for the given x, y and z scale values
auto scale(const vec3_t &vec3) -> mat4_t;

// returns the scale matrix for the given (same) x, y and z scale values
auto scale(float x) -> mat4_t;

// returns the rotation matrix for the given angle and axis
auto rotate(float angle, const vec3_t &axis) -> mat4_t;

// returns the normalized vector
auto normalize(const vec3_t &axis) -> vec3_t;

// returns the view matrix
auto get_view_matrix(const transform_t &transform) -> mat4_t;

// returns the cross product of the given two vectors
auto cross(const vec3_t &first, const vec3_t &second) -> vec3_t;

// returns the look at matrix
auto look_at(const vec3_t &eye, const vec3_t &centre, const vec3_t &up) -> mat4_t;

// returns the perspective matrix
auto perspective(float fov_y, float aspect, float z_near, float z_far) -> mat4_t;

// returns the orthographic matrix
auto orthographic(float left, float right, float bottom, float top, float near, float far) -> mat4_t;
} // namespace aech::math // namespace aech::math

// namespace aech::math
