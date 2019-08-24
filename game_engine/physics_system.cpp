#include "physics_system.hpp"
#include "engine.hpp"
#include "main.hpp"
#include "scene_node.hpp"
#include "rigid_body.hpp"
#include "transform.hpp"

namespace aech
{
	void physics_system_t::update(float dt)
	{
		for (const auto &entity: entities)
		{
			auto& transform = engine.get_component<transform_t>(entity);
			auto& rigid_body = engine.get_component<rigid_body_t>(entity);

			transform.position += rigid_body.velocity * dt;
			rigid_body.velocity += rigid_body.acceleration * dt;
		}
	}
}
