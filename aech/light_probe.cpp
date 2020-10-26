#include "light_probe.hpp"

aech::graphics::light_probe_t::light_probe_t(math::vec3_t  position,
                                             float         inner_radius,
                                             float         outer_radius,
                                             scene_node_t *scene_node)
    : m_scene_node{scene_node}
    , m_position{position}
    , m_inner_radius{inner_radius}
    , m_outer_radius{outer_radius}
{
}

auto aech::graphics::light_probe_t::irradiance() const -> aech::graphics::texture_cube_t *
{
    return m_irradiance;
}

auto aech::graphics::light_probe_t::position() const -> aech::math::vec3_t
{
    return m_position;
}

auto aech::graphics::light_probe_t::inner_radius() const -> float
{
    return m_inner_radius;
}

auto aech::graphics::light_probe_t::outer_radius() const -> float
{
    return m_outer_radius;
}

auto aech::graphics::light_probe_t::scene_node() const -> aech::graphics::scene_node_t *
{
    return m_scene_node;
}

auto aech::graphics::light_probe_t::set_irradiance(texture_cube_t *irradiance) -> void
{
    m_irradiance = irradiance;
}
