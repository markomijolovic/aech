#pragma once

#include "aech_types.hpp"
#include "mat.hpp"
#include "transform.hpp"
#include "vec.hpp"

namespace aech::math {
// computes the angle in radians from the provided angle which is assumed to be in degrees
[[nodiscard]] constexpr auto radians(float angle) noexcept
{
    return angle * (pi / 180.0F);
}

// linearly interpolates between a and b
[[nodiscard]] constexpr auto lerp(float a, float b, float t) noexcept
{
    return a + t * (b - a);
}

// returns the translation matrix for the given x, y and z translation values
[[nodiscard]] auto translate(float x, float y, float z) noexcept -> mat4_t;

// returns the translation matrix for the given x, y and z translation values
[[nodiscard]] auto translate(const vec3_t &vec3) noexcept -> mat4_t;

// returns the scale matrix for the given x, y and z scale values
auto scale(float x, float y, float z) noexcept -> mat4_t;

// returns the scale matrix for the given x, y and z scale values
[[nodiscard]] auto scale(const vec3_t &vec3) noexcept -> mat4_t;

// returns the scale matrix for the given (same) x, y and z scale values
[[nodiscard]] auto scale(float x) noexcept -> mat4_t;

// returns the rotation matrix for the given angle and axis
[[nodiscard]] auto rotate(float angle, const vec3_t &axis) noexcept -> mat4_t;

// returns the normalized vector
[[nodiscard]] auto normalize(const vec3_t &axis) noexcept -> vec3_t;

// returns the view matrix
[[nodiscard]] auto get_view_matrix(const transform_t &transform) noexcept -> mat4_t;

// returns the cross product of the given two vectors
[[nodiscard]] auto cross(const vec3_t &first, const vec3_t &second) noexcept -> vec3_t;

// returns the look at matrix
[[nodiscard]] auto look_at(const vec3_t &eye, const vec3_t &centre, const vec3_t &up) noexcept -> mat4_t;

// returns the perspective matrix
[[nodiscard]] auto perspective(float fov_y, float aspect, float z_near, float z_far) noexcept -> mat4_t;

// returns the orthographic matrix
[[nodiscard]] auto orthographic(float left, float right, float bottom, float top, float near, float far) noexcept -> mat4_t;
} // namespace aech::math // namespace aech::math

// namespace aech::math
