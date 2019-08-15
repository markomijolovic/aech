#include "engine.hpp"

namespace aech
{
	void engine_t::init()
	{
		m_component_manager = std::make_unique<component_manager_t>();
		m_entity_manager    = std::make_unique<entity_manager_t>();
		m_system_manager    = std::make_unique<system_manager_t>();
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
} // namespace aech
