#include "physics_system.hpp"
#include "engine.hpp"
#include "components.hpp"

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
			auto& rigid_body = engine.get_component<rigid_body_t>(entity);
			auto& transform = engine.get_component<transform_t>(entity);

			const auto& gravity = engine.get_component<gravity_t>(entity);
			transform.position += rigid_body.velocity * dt;

			// ?
			rigid_body.velocity += gravity.force * dt;
		}
	}
}
