#include "system_manager.hpp"

namespace aech::ecs
{
	void system_manager_t::entity_destroyed(entity_t entity)
	{
		for (const auto& el : m_systems)
		{
			el.second->m_entities.erase(entity);
		}
	}

	void system_manager_t::entity_signature_changed(entity_t entity, signature_t entity_signature)
	{
		for (const auto& el : m_systems)
		{
			const auto& type             = el.first;
			const auto& system           = el.second;
			const auto& system_signature = m_signatures[type];

			if ((entity_signature & system_signature) == system_signature)
			{
				system->m_entities.insert(entity);
			}
			else
			{
				system->m_entities.erase(entity);
			}
		}
	}
} // namespace aech
