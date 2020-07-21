#include "scene_node.hpp"
#include "main.hpp"
#include "mat.hpp"
#include "transform.hpp"

namespace aech::graphics {
scene_node_t::scene_node_t(transform_t* transform, scene_node_t* parent)
    : m_parent { parent }
    , m_transform { transform }
{
}

void scene_node_t::set_position(const math::vec3_t& position)
{
    m_transform->position = position;
    m_dirty = true;
    for (auto& child : m_children) {
        child->parent_changed();
    }
}

void scene_node_t::parent_changed() const
{
    m_dirty = true;
    for (const auto& child : m_children) {
        child->parent_changed();
    }
}

void scene_node_t::move(const math::vec3_t& offset) const
{
    m_transform->position += offset;
    m_dirty = true;
}

void scene_node_t::set_rotation(const math::vec3_t& rotation) const
{
    m_transform->rotation = rotation;
    m_dirty = true;
}

void scene_node_t::set_scale(const math::vec3_t& scale) const
{
    m_transform->scale = scale;
    m_dirty = true;
}

void scene_node_t::set_scale(float scale) const
{
    m_transform->scale = { scale, scale, scale };
    m_dirty = true;
}

math::vec3_t scene_node_t::get_local_position() const
{
    return m_transform->position;
}

math::vec3_t scene_node_t::get_local_scale() const
{
    return m_transform->scale;
}

math::vec3_t scene_node_t::get_local_rotation() const
{
    return m_transform->rotation;
}

math::vec3_t scene_node_t::get_world_position() const
{
    const auto transform_matrix = get_transform();
    const auto pos = transform_matrix * math::vec4_t { m_transform->position, 1.0F };
    return math::vec3_t { pos };
}

math::mat4_t scene_node_t::get_transform() const
{
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

math::vec3_t scene_node_t::get_world_scale() const
{
    auto transform = get_transform();
    return { transform[0][0], transform[1][1], transform[2][2] };
}

bounding_box_t scene_node_t::bounding_box() const
{
    // TODO(Marko): how to transform aabb?

    const auto transform_matrix = get_transform();
    return {
        transform_matrix * m_aabb.min_coords,
        transform_matrix * m_aabb.max_coords
    };
}

void scene_node_t::add_child(scene_node_t* node)
{
    if (node->m_parent != nullptr) {
        node->m_parent->remove_child(node);
    }
    node->m_parent = this;
    m_children.push_back(node);
}

void scene_node_t::remove_child(scene_node_t* node)
{
    const auto it = std::find(std::begin(m_children), std::end(m_children), node);
    if (it != std::end(m_children)) {
        m_children.erase(it);
    }
}

void scene_node_t::set_aabb(const bounding_box_t& aabb)
{
    m_aabb = aabb;
}
} // namespace aech::graphics
