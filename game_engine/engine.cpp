#include "engine.hpp"
#include "scene_node.hpp"

namespace aech
{
	engine_t::engine_t()
		:
		m_component_manager{ std::make_unique<component_manager_t>() },
		m_entity_manager{ std::make_unique<entity_manager_t>() },
		m_system_manager{ std::make_unique<system_manager_t>() },
		m_event_manager{ std::make_unique<event_manager_t>() },
		m_root_node{create_entity() }
	{
		register_component<scene_node_t>();
		add_component(
			m_root_node,
			std::move(scene_node_t{m_root_node})
		);
	}

	entity_t engine_t::create_entity() const
	{
		return m_entity_manager->create_entity();
	}

	void engine_t::destroy_entity(entity_t entity) const
	{
		m_entity_manager->destroy_entity(entity);
		m_component_manager->entity_destroyed(entity);
		m_system_manager->entity_destroyed(entity);
	}


	void engine_t::add_event_listener(event_id_t event_id, const std::function<void(event_t&)>& listener)
	{
		m_event_manager->add_listener(event_id, listener);
	}

	void engine_t::send_event(event_t& event)
	{
		m_event_manager->send_event(event);
	}

	void engine_t::send_event(event_id_t event_id)
	{
		m_event_manager->send_event(event_id);
	}
} // namespace aech
