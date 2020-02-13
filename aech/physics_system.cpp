#include "physics_system.hpp"
#include "main.hpp"
#include "rigidbody.hpp"

void aech::physics::physics_system_t::update(float /*delta_time*/)
{
	for (auto entity : m_entities)
	{
		auto& rigidbody  = engine.get_component<rigidbody_t>(entity);
		auto& scene_node = engine.get_component<graphics::scene_node_t>(entity);

		if (rigidbody.gravity())
		{
		}
	}
}
