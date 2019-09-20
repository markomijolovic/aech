#include "engine.hpp"
#include "shading_tags.hpp"
#include "shadow_caster.hpp"
#include "point_light.hpp"
#include "directional_light.hpp"
#include "mesh_filter.hpp"
#include "camera.hpp"
#include "scene_node.hpp"

namespace aech
{
	void engine_t::init()
	{
		register_component<transform_t>();
		register_component<graphics::scene_node_t>();
		register_component<camera_t>();
		register_component<graphics::mesh_filter_t>();
		register_component<graphics::directional_light_t>();
		register_component<graphics::point_light_t>();
		register_component<graphics::shadow_caster_t>();
		register_component<graphics::opaque_t>();
		register_component<graphics::transparent_t>();
	}

	entity_t engine_t::create_entity()
	{
		return m_entity_manager.create_entity();
	}

	void engine_t::destroy_entity(entity_t entity)
	{
		m_entity_manager.destroy_entity(entity);
		m_component_manager.entity_destroyed(entity);
		m_system_manager.entity_destroyed(entity);
	}


	void engine_t::add_event_listener(event_id_t event_id, const std::function<void(events::event_t&)>& listener)
	{
		m_event_manager.add_listener(event_id, listener);
	}

	void engine_t::send_event(events::event_t& event)
	{
		m_event_manager.send_event(event);
	}
} // namespace aech
