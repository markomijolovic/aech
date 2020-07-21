#pragma once

#include "mat.hpp"

namespace aech {
// transform component
struct transform_t {
    math::vec3_t position {};
    math::vec3_t rotation {};
    math::vec3_t scale { 1.0F, 1.0F, 1.0F };

    [[nodiscard]] math::mat4_t get_transform_matrix() const;
    [[nodiscard]] math::vec3_t get_forward_vector() const;
};
} // namespace aech
