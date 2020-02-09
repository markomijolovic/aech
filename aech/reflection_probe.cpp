#include "reflection_probe.hpp"


//aech::graphics::texture_cube_t* aech::graphics::reflection_probe_t::irradiance() const
//{
//	return m_irradiance;
//}

aech::graphics::texture_cube_t* aech::graphics::reflection_probe_t::prefiltered() const
{
	return m_prefiltered;
}

aech::math::vec3_t aech::graphics::reflection_probe_t::position() const
{
	return m_position;
}

aech::graphics::reflection_probe_t::reflection_probe_t(math::vec3_t position,
	scene_node_t* scene_node)
	:
	m_scene_node{scene_node},
	m_position{position}
{
}

aech::graphics::scene_node_t* aech::graphics::reflection_probe_t::scene_node() const
{
	return m_scene_node;
}

void aech::graphics::reflection_probe_t::set_prefiltered(texture_cube_t* const prefiltered)
{
	m_prefiltered = prefiltered;
}
