#include "component_manager.hpp"

namespace aech::ecs
{
	void component_manager_t::entity_destroyed(entity_t entity)
	{
		for (const auto& el : m_component_arrays)
		{
			el.second->entity_destroyed(entity);
		}
	}
} // namespace aech
