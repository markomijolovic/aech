#include "light_probe.hpp"


aech::graphics::light_probe_t::light_probe_t(math::vec3_t m_position, float m_radius, scene_node_t *scene_node) :
	m_position{m_position},
	m_radius{m_radius},
	m_scene_node{scene_node}
{
}

aech::graphics::texture_cube_t* aech::graphics::light_probe_t::irradiance() const
{
	return m_irradiance;
}

aech::graphics::texture_cube_t* aech::graphics::light_probe_t::prefiltered() const
{
	return m_prefiltered;
}

aech::math::vec3_t aech::graphics::light_probe_t::position() const
{
	return m_position;
}

float aech::graphics::light_probe_t::radius() const
{
	return m_radius;
}


aech::graphics::scene_node_t* aech::graphics::light_probe_t::scene_node() const
{
	return m_scene_node;
}

void aech::graphics::light_probe_t::set_irradiance(texture_cube_t* const irradiance)
{
	m_irradiance = irradiance;
}

void aech::graphics::light_probe_t::set_prefiltered(texture_cube_t* const prefiltered)
{
	m_prefiltered = prefiltered;
}
