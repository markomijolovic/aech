#include "light_probe.hpp"

aech::graphics::light_probe_t::light_probe_t(math::vec3_t position,
    float inner_radius,
    float outer_radius,
    scene_node_t* scene_node)
    : m_scene_node { scene_node }
    , m_position { position }
    , m_inner_radius { inner_radius }
    , m_outer_radius { outer_radius }
{
}

aech::graphics::texture_cube_t* aech::graphics::light_probe_t::irradiance() const
{
    return m_irradiance;
}

aech::math::vec3_t aech::graphics::light_probe_t::position() const
{
    return m_position;
}

float aech::graphics::light_probe_t::inner_radius() const
{
    return m_inner_radius;
}

float aech::graphics::light_probe_t::outer_radius() const
{
    return m_outer_radius;
}

aech::graphics::scene_node_t* aech::graphics::light_probe_t::scene_node() const
{
    return m_scene_node;
}

void aech::graphics::light_probe_t::set_irradiance(texture_cube_t* irradiance)
{
    m_irradiance = irradiance;
}
