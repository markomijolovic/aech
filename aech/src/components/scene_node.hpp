#pragma once

#include "aech_types.hpp"
#include "bounding_box.hpp"
#include "mesh.hpp"
#include "transform.hpp"

namespace aech::graphics {
// a class that represents a node in the scene
// TODO: optimize transform computation with caching
class scene_node_t {
public:
    [[nodiscard]] auto get_local_position() const noexcept -> math::vec3_t;
    [[nodiscard]] auto get_local_scale() const noexcept -> math::vec3_t;
    [[nodiscard]] auto get_local_rotation() const noexcept -> math::vec3_t;
    [[nodiscard]] auto get_transform() const noexcept -> math::mat4_t;
    [[nodiscard]] auto get_world_position() const noexcept -> math::vec3_t;
    [[nodiscard]] auto get_world_scale() const noexcept -> math::vec3_t;
    [[nodiscard]] auto bounding_box() const noexcept -> bounding_box_t;
    auto               set_position(const math::vec3_t &position) noexcept -> void;
    auto               set_rotation(const math::vec3_t &rotation) const noexcept -> void;
    auto               set_scale(const math::vec3_t &scale) const noexcept -> void;
    auto               set_scale(float scale) const noexcept -> void;
    auto               parent_changed() const noexcept -> void;
    auto               move(const math::vec3_t &offset) const noexcept -> void;

    scene_node_t() = default;
    explicit scene_node_t(transform_t *transform, scene_node_t *parent = nullptr) noexcept;

    auto add_child(scene_node_t *node) noexcept -> void;
    auto remove_child(scene_node_t *node) noexcept -> void;

    auto set_aabb(const bounding_box_t &aabb) noexcept -> void;

private:
    transform_t *  m_transform{};
    bounding_box_t m_aabb{};

    // NOTE: these are all non-owning pointers!!!
    std::vector<scene_node_t *> m_children{};
    scene_node_t *              m_parent{};

    // transform cache
    mutable math::mat4_t m_transform_matrix{};
    mutable bool         m_dirty{true};
};
} // namespace aech::graphics
