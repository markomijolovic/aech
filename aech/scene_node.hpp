#pragma once

#include "bounding_box.hpp"
#include "mesh.hpp"
#include "transform.hpp"

namespace aech::graphics {
// a class that represents a node in the scene
// todo: optimize transform computation with caching
class scene_node_t {
public:
    [[nodiscard]] auto get_local_position() const -> math::vec3_t;
    [[nodiscard]] auto get_local_scale() const -> math::vec3_t;
    [[nodiscard]] auto get_local_rotation() const -> math::vec3_t;
    [[nodiscard]] auto get_transform() const -> math::mat4_t;
    [[nodiscard]] auto get_world_position() const -> math::vec3_t;
    [[nodiscard]] auto get_world_scale() const -> math::vec3_t;
    [[nodiscard]] auto bounding_box() const -> bounding_box_t;
    auto               set_position(const math::vec3_t &position) -> void;
    auto               set_rotation(const math::vec3_t &rotation) const -> void;
    auto               set_scale(const math::vec3_t &scale) const -> void;
    auto               set_scale(float scale) const -> void;
    auto               parent_changed() const -> void;
    auto               move(const math::vec3_t &offset) const -> void;

    // make array happy
    scene_node_t() = default;
    explicit scene_node_t(transform_t *transform, scene_node_t *parent = nullptr);

    auto add_child(scene_node_t *node) -> void;
    auto remove_child(scene_node_t *node) -> void;

    auto set_aabb(const bounding_box_t &aabb) -> void;

private:
    std::vector<scene_node_t *> m_children{};
    bounding_box_t              m_aabb{};
    scene_node_t *              m_parent{};
    transform_t *               m_transform{};
    // transform cache
    mutable math::mat4_t m_transform_matrix{};
    mutable bool         m_dirty{true};
};
} // namespace aech::graphics
