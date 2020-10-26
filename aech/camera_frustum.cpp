#include "camera_frustum.hpp"

#include "bounding_box.hpp"
#include "cube.hpp"
#include "vector_math.hpp"

auto aech::camera_frustum_t::recalculate(const math::mat4_t &pv) -> void
{
    const math::vec4_t m0{
        pv[0][0],
        pv[0][1],
        pv[0][2],
        pv[0][3]};
    const math::vec4_t m1{
        pv[1][0],
        pv[1][1],
        pv[1][2],
        pv[1][3]};
    const math::vec4_t m2{
        pv[2][0],
        pv[2][1],
        pv[2][2],
        pv[2][3]};
    const math::vec4_t m3{
        pv[3][0],
        pv[3][1],
        pv[3][2],
        pv[3][3]};

    planes[0] = m0 + m3;
    planes[1] = m3 - m0;
    planes[2] = m3 + m1;
    planes[3] = m3 - m1;
    planes[4] = m3 + m2;
    planes[5] = m3 - m2;
}

auto aech::camera_frustum_t::intersects(const math::vec3_t &centre, float radius) const -> bool
{
    for (auto &plane: planes) {
        std::int32_t count{};
        if (dot(math::vec4_t{centre, 1.0F}, plane) >= -radius) {
            count++;
        }
        if (count == 0) {
            return false;
        }
    }

    return true;
}

auto aech::camera_frustum_t::intersects(const graphics::bounding_box_t &aabb) const -> bool
{
    // get all vertices of bounding box
    std::array vertices{
        aabb.min_coords,
        math::vec4_t{aabb.max_coords.x, aabb.min_coords.y, aabb.min_coords.z, 1.0F},
        math::vec4_t{aabb.min_coords.x, aabb.max_coords.y, aabb.min_coords.z, 1.0F},
        math::vec4_t{aabb.max_coords.x, aabb.max_coords.y, aabb.min_coords.z, 1.0F},
        math::vec4_t{aabb.min_coords.x, aabb.min_coords.y, aabb.max_coords.z, 1.0F},
        math::vec4_t{aabb.max_coords.x, aabb.min_coords.y, aabb.max_coords.z, 1.0F},
        math::vec4_t{aabb.min_coords.x, aabb.max_coords.y, aabb.max_coords.z, 1.0F},
        aabb.max_coords};

    // test all vertices against all planes
    for (const auto &plane: planes) {
        std::int32_t count{};
        for (auto &vertex: vertices) {
            if (dot(vertex, plane) > 0) {
                count++;
            }
        }
        if (count == 0) {
            return false;
        }
    }

    return true;
}
