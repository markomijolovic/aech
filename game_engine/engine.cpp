#include "engine.hpp"

namespace aech
{
	engine_t::engine_t() :
		m_component_manager{std::make_unique<ecs::component_manager_t>()},
		m_entity_manager{std::make_unique<ecs::entity_manager_t>()},
		m_system_manager{std::make_unique<ecs::system_manager_t>()},
		m_event_manager{std::make_unique<events::event_manager_t>()}
	{
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


	void engine_t::add_event_listener(event_id_t event_id, const std::function<void(events::event_t&)>& listener) const
	{
		m_event_manager->add_listener(event_id, listener);
	}

	void engine_t::send_event(events::event_t& event) const
	{
		m_event_manager->send_event(event);
	}
} // namespace aech
