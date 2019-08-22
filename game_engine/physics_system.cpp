#include "physics_system.hpp"
#include "engine.hpp"
#include "components.hpp"
#include "scene_node.hpp"

extern aech::engine_t engine;

namespace aech
{
	void physics_system_t::init()
	{
	}

	void physics_system_t::update(float dt)
	{
		for (const auto &entity: m_entities)
		{
			auto& scene_node = engine.get_component<scene_node_t>(entity);
			auto& rigid_body = engine.get_component<rigid_body_t>(entity);

			const auto& gravity = engine.get_component<gravity_t>(entity);
			scene_node.m_ditry = true;
			scene_node.m_position += rigid_body.velocity * dt;

			rigid_body.velocity += gravity.force * dt;
		}
	}
}
