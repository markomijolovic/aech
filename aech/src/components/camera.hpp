#pragma once

#include "aech_types.hpp"
#include "camera_frustum.hpp"
#include "mat.hpp"
#include "scene_node.hpp"
#include "transform.hpp"

namespace aech {
// camera component
class camera_t {
public:
    camera_t() = default; // make map happy
    camera_t(transform_t *transform, const math::mat4_t &projection, camera_frustum_t frustum = {}) noexcept;

    auto rotate_x(float amount) noexcept -> void;
    auto rotate_y(float amount) noexcept -> void;
    auto translate(const math::vec3_t &amount) noexcept -> void;

    // does this camera "see" this mesh?
    [[nodiscard]] auto sees(const graphics::scene_node_t &scene_node) const noexcept -> bool;
    [[nodiscard]] auto sees(const math::vec3_t &centre, float radius) const noexcept -> bool;

    [[nodiscard]] auto frustum() const noexcept -> const camera_frustum_t &;
    [[nodiscard]] auto view_matrix() const noexcept -> math::mat4_t;
    [[nodiscard]] auto projection() const noexcept -> math::mat4_t;
    [[nodiscard]] auto transform() const noexcept -> transform_t &;

private:
    // NOTE: this is a NON-OWNING pointer!!!
    // it can not be a reference because we require components to be default constructible and we can not determine the proper object to refer to at construction time
    transform_t *            m_transform{};
    math::mat4_t             m_projection{};
    mutable bool             m_dirty{true};
    mutable camera_frustum_t m_frustum{};

    auto recalculate_frustum() const noexcept -> void;
};
} // namespace aech
