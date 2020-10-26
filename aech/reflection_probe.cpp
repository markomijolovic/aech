#include "reflection_probe.hpp"

auto aech::graphics::reflection_probe_t::prefiltered() const -> aech::graphics::texture_cube_t *
{
    return m_prefiltered;
}

auto aech::graphics::reflection_probe_t::position() const -> aech::math::vec3_t
{
    return m_position;
}

aech::graphics::reflection_probe_t::reflection_probe_t(math::vec3_t  position,
                                                       scene_node_t *scene_node)
    : m_scene_node{scene_node}
    , m_position{position}
{
}

auto aech::graphics::reflection_probe_t::scene_node() const -> aech::graphics::scene_node_t *
{
    return m_scene_node;
}

auto aech::graphics::reflection_probe_t::set_prefiltered(texture_cube_t *const prefiltered) -> void
{
    m_prefiltered = prefiltered;
}
