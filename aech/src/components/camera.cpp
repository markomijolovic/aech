#include "camera.hpp"

#include "main.hpp"

auto aech::camera_t::rotate_x(float amount) noexcept -> void
{
    assert(m_transform);

    m_transform->rotation.x += amount;
    m_transform->rotation.x = std::clamp(m_transform->rotation.x, -89.0F, 89.0F);
    m_dirty                 = true;
}

auto aech::camera_t::frustum() const noexcept -> const camera_frustum_t &
{
    if (m_dirty) {
        recalculate_frustum();
        m_dirty = false;
    }
    return m_frustum;
}

aech::camera_t::camera_t(transform_t *transform, const math::mat4_t &projection, camera_frustum_t frustum) noexcept
    : m_transform{transform}
    , m_projection{projection}
    , m_frustum{frustum}
{
}

auto aech::camera_t::projection() const noexcept -> math::mat4_t
{
    return m_projection;
}

auto aech::camera_t::transform() const noexcept -> transform_t &
{
    assert(m_transform);

    return *m_transform;
}

auto aech::camera_t::view_matrix() const noexcept -> math::mat4_t
{
    assert(m_transform);

    return math::get_view_matrix(*m_transform);
}

auto aech::camera_t::sees(const graphics::scene_node_t &scene_node) const noexcept -> bool
{
    return frustum().intersects(scene_node.bounding_box());
}

auto aech::camera_t::sees(const math::vec3_t &centre, float radius) const noexcept -> bool
{
    return frustum().intersects(centre, radius);
}

auto aech::camera_t::translate(const math::vec3_t &amount) noexcept -> void
{
    assert(m_transform);

    m_transform->position += amount;
    m_dirty = true;
}

auto aech::camera_t::rotate_y(float amount) noexcept -> void
{
    assert(m_transform);

    m_dirty = true;
    m_transform->rotation.y += amount;
    m_transform->rotation.y = std::fmod(m_transform->rotation.y, 360.0F);
}

auto aech::camera_t::recalculate_frustum() const noexcept -> void
{
    assert(m_transform);

    const auto pv = m_projection * math::get_view_matrix(*m_transform);
    m_frustum.recalculate(pv);
}
