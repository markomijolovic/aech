#include "entity_manager.hpp"

aech::ecs::entity_manager_t::entity_manager_t()
{
	for (entity_t entity = 0; entity < max_entities; entity++)
	{
		m_available_entities.push(entity);
	}
}

aech::entity_t aech::ecs::entity_manager_t::create_entity()
{
	auto id = m_available_entities.front();

	m_available_entities.pop();
	m_living_entities_count++;

	return id;
}

void aech::ecs::entity_manager_t::destroy_entity(entity_t entity)
{
	m_signatures[entity].reset();
	m_available_entities.push(entity);
	m_living_entities_count--;
}

void aech::ecs::entity_manager_t::set_signature(entity_t entity, signature_t signature)
{
	m_signatures[entity] = signature;
}

aech::signature_t aech::ecs::entity_manager_t::get_signature(entity_t entity)
{
	return m_signatures[entity];
}
