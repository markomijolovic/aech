#pragma once

#include "mat.hpp"

namespace aech {
// transform component
struct transform_t {
    math::vec3_t position{};
    math::vec3_t rotation{};
    math::vec3_t scale{1.0F, 1.0F, 1.0F};

    [[nodiscard]] auto get_transform_matrix() const noexcept -> math::mat4_t;
    [[nodiscard]] auto get_forward_vector() const noexcept -> math::vec3_t;
};
} // namespace aech
