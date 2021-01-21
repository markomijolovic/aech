#include "scene_node.hpp"

namespace aech::graphics {
scene_node_t::scene_node_t(transform_t *transform, scene_node_t *parent) noexcept
    : m_transform{transform}
    , m_parent{parent}
{
}

auto scene_node_t::set_position(const math::vec3_t &position) noexcept -> void
{
    assert(m_transform);
    m_transform->position = position;
    m_dirty               = true;
    for (auto &child: m_children) {
        child->parent_changed();
    }
}

auto scene_node_t::parent_changed() const noexcept -> void
{
    m_dirty = true;
    for (const auto &child: m_children) {
        child->parent_changed();
    }
}

auto scene_node_t::move(const math::vec3_t &offset) const noexcept -> void
{
    assert(m_transform);

    m_transform->position += offset;
    m_dirty = true;
}

auto scene_node_t::set_rotation(const math::vec3_t &rotation) const noexcept -> void
{
    assert(m_transform);

    m_transform->rotation = rotation;
    m_dirty               = true;
}

auto scene_node_t::set_scale(const math::vec3_t &scale) const noexcept -> void
{
    assert(m_transform);

    m_transform->scale = scale;
    m_dirty            = true;
}

auto scene_node_t::set_scale(float scale) const noexcept -> void
{
    assert(m_transform);

    m_transform->scale = {scale, scale, scale};
    m_dirty            = true;
}

auto scene_node_t::get_local_position() const noexcept -> math::vec3_t
{
    assert(m_transform);

    return m_transform->position;
}

auto scene_node_t::get_local_scale() const noexcept -> math::vec3_t
{
    assert(m_transform);

    return m_transform->scale;
}

auto scene_node_t::get_local_rotation() const noexcept -> math::vec3_t
{
    assert(m_transform);

    return m_transform->rotation;
}

auto scene_node_t::get_world_position() const noexcept -> math::vec3_t
{
    const auto transform_matrix = get_transform();
    const auto pos              = transform_matrix * math::vec4_t{m_transform->position, 1.0F};
    return math::vec3_t{pos};
}

auto scene_node_t::get_transform() const noexcept -> math::mat4_t
{
    assert(m_transform);

    if (!m_dirty) {
        return m_transform_matrix;
    }

    m_transform_matrix = m_transform->get_transform_matrix();

    if (m_parent != nullptr) {
        m_transform_matrix = m_parent->get_transform() * m_transform_matrix;
    }

    m_dirty = false;

    return m_transform_matrix;
}

auto scene_node_t::get_world_scale() const noexcept -> math::vec3_t
{
    auto transform = get_transform();
    return {transform[0][0], transform[1][1], transform[2][2]};
}

auto scene_node_t::bounding_box() const noexcept -> bounding_box_t
{
    // TODO(Marko): how to transform aabb?

    const auto transform_matrix = get_transform();
    return {
        transform_matrix * m_aabb.min_coords,
        transform_matrix * m_aabb.max_coords};
}

auto scene_node_t::add_child(scene_node_t *node) noexcept -> void
{
    assert(node != nullptr);
    assert(node->m_parent == nullptr);

    node->m_parent = this;
    m_children.push_back(node);
}

auto scene_node_t::remove_child(scene_node_t *node) noexcept -> void
{
    assert(node != nullptr);

    if (const auto it = std::find(std::begin(m_children), std::end(m_children), node); it != std::end(m_children)) {
        m_children.erase(it);
    }
}

auto scene_node_t::set_aabb(const bounding_box_t &aabb) noexcept -> void
{
    m_aabb = aabb;
}
} // namespace aech::graphics
