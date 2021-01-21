#include "transforms.hpp"

#include <cmath>

namespace aech::math {
auto translate(float x, float y, float z) noexcept -> mat4_t
{
    mat4_t retval{};
    retval[0][3] = x;
    retval[1][3] = y;
    retval[2][3] = z;
    return retval;
}

auto scale(float x, float y, float z) noexcept -> mat4_t
{
    mat4_t retval{};
    retval[0][0] *= x;
    retval[1][1] *= y;
    retval[2][2] *= z;
    return retval;
}

auto length(const vec3_t &axis) noexcept -> float
{
    return std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
}

auto normalize(const vec3_t &axis) noexcept -> vec3_t
{
    return axis * (1 / length(axis));
}

auto rotate(float angle, const vec3_t &axis_) noexcept -> mat4_t
{
    const auto axis = normalize(axis_);
    const auto s    = std::sin(angle);
    const auto c    = std::cos(angle);
    const auto oc   = 1.0F - c;

    return mat4_t{
        oc * axis.x * axis.x + c,
        oc * axis.x * axis.y - axis.z * s,
        oc * axis.z * axis.x + axis.y * s,
        0.0,
        oc * axis.x * axis.y + axis.z * s,
        oc * axis.y * axis.y + c,
        oc * axis.y * axis.z - axis.x * s,
        0.0,
        oc * axis.z * axis.x - axis.y * s,
        oc * axis.y * axis.z + axis.x * s,
        oc * axis.z * axis.z + c,
        0.0,
        0.0,
        0.0,
        0.0,
        1.0};
}

auto get_view_matrix(const transform_t &transform) noexcept -> mat4_t
{
    const auto mat_pitch = rotate(radians(-transform.rotation.x), {1, 0, 0});
    const auto mat_yaw   = rotate(radians(-transform.rotation.y), {0, 1, 0});
    const auto mat_roll  = rotate(radians(-transform.rotation.z), {0, 0, 1});
    const auto rotate    = mat_pitch * mat_yaw * mat_roll;
    const auto trans     = translate(-transform.position.x, -transform.position.y, -transform.position.z);
    return rotate * trans;
}

auto translate(const vec3_t &vec3) noexcept -> mat4_t
{
    return translate(vec3.x, vec3.y, vec3.z);
}

auto scale(const vec3_t &vec3) noexcept -> mat4_t
{
    return scale(vec3.x, vec3.y, vec3.z);
}

auto scale(float x) noexcept -> mat4_t
{
    return scale(x, x, x);
}

auto look_at(const vec3_t &eye, const vec3_t &centre, const vec3_t &up) noexcept -> mat4_t
{
    const auto w          = normalize(eye - centre);
    const auto u          = normalize(cross(up, w));
    const auto v          = normalize(cross(w, u));
    const auto first_mat  = mat4_t{u.x, u.y, u.z, 0, v.x, v.y, v.z, 0, w.x, w.y, w.z, 0, 0, 0, 0, 1};
    const auto second_mat = mat4_t{
        1,
        0,
        0,
        -eye.x,
        0,
        1,
        0,
        -eye.y,
        0,
        0,
        1,
        -eye.z,
        0,
        0,
        0,
        1};
    return first_mat * second_mat;
}

auto perspective(float fov_y, float aspect, float z_near, float z_far) noexcept -> mat4_t
{
    const auto theta = radians(fov_y / 2);
    const auto d     = cos(theta) / sin(theta);
    const auto a     = -(z_far + z_near) / (z_far - z_near);
    const auto b     = -(2 * z_far * z_near) / (z_far - z_near);
    return mat4_t{
        static_cast<float>(d / aspect),
        0,
        0,
        0,
        0,
        static_cast<float>(d),
        0,
        0,
        0,
        0,
        a,
        b,
        0,
        0,
        -1,
        0};
}

auto cross(const vec3_t &first, const vec3_t &second) noexcept -> vec3_t
{
    return vec3_t{
        first.y * second.z - first.z * second.y,
        -(first.x * second.z - first.z * second.x),
        first.x * second.y - first.y * second.x};
}

auto orthographic(float left, float right, float bottom, float top, float near, float far) noexcept -> mat4_t
{
    return mat4_t{
        {2 / (right - left), 0, 0, -(right + left) / (right - left)},
        {0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom)},
        {0, 0, -2 / (far - near), -(far + near) / (far - near)},
        {0, 0, 0, 1}};
}
} // namespace aech::math
