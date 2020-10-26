#pragma once

#include "camera_frustum.hpp"
#include "mat.hpp"
#include "scene_node.hpp"
#include "transform.hpp"

namespace aech {
// camera component
class camera_t {
public:
    camera_t() = default;
    camera_t(const math::mat4_t &projection, transform_t *transform, camera_frustum_t frustum = {});
    auto frustum() -> const camera_frustum_t &;
    auto rotate_x(float amount) -> void;
    auto rotate_y(float amount) -> void;
    auto translate(const math::vec3_t &amount) -> void;
    // does this camera "see" this mesh?
    auto               sees(const graphics::scene_node_t &scene_node) -> bool;
    auto               sees(const math::vec3_t &centre, float radius) -> bool;
    [[nodiscard]] auto view_matrix() const -> math::mat4_t;
    [[nodiscard]] auto projection() const -> math::mat4_t;
    [[nodiscard]] auto transform() const -> transform_t *;

private:
    math::mat4_t     m_projection{};
    camera_frustum_t m_frustum{};
    transform_t *    m_transform{};
    bool             m_dirty{true};
    auto             recalculate_frustum() -> void;
};
} // namespace aech
