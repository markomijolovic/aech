#include "transform.hpp"

#include "transforms.hpp"

auto aech::transform_t::get_transform_matrix() const noexcept -> math::mat4_t
{
    auto transform_matrix = translate(position);
    transform_matrix *= math::rotate(rotation.x, {1.0F, 0.0F, 0.0F});
    transform_matrix *= math::rotate(rotation.y, {0.0F, 1.0F, 0.0F});
    transform_matrix *= math::rotate(rotation.z, {0.0F, 0.0F, 1.0F});
    transform_matrix *= math::scale(scale);

    return transform_matrix;
}

auto aech::transform_t::get_forward_vector() const noexcept -> math::vec3_t
{
    math::vec4_t forward = math::rotate(math::radians(rotation.x), {1, 0, 0}) * math::vec4_t{0, 0, -1, 0};
    forward              = math::rotate(math::radians(rotation.y), {0, 1, 0}) * forward;
    forward              = math::rotate(math::radians(rotation.z), {0, 0, 1}) * forward;
    return math::vec3_t{forward};
}
