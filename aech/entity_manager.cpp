#include "entity_manager.hpp"

aech::ecs::entity_manager_t::entity_manager_t()
{
    for (entity_t entity = 0; entity < max_entities; entity++) {
        m_available_entities.push(entity);
    }
}

auto aech::ecs::entity_manager_t::create_entity() -> aech::entity_t
{
    const auto id = m_available_entities.front();

    m_available_entities.pop();
    m_living_entities_count++;

    return id;
}

auto aech::ecs::entity_manager_t::destroy_entity(entity_t entity) -> void
{
    m_signatures[entity].reset();
    m_available_entities.push(entity);
    m_living_entities_count--;
}

auto aech::ecs::entity_manager_t::set_signature(entity_t entity, signature_t signature) -> void
{
    m_signatures[entity] = signature;
}

auto aech::ecs::entity_manager_t::get_signature(entity_t entity) -> aech::signature_t
{
    return m_signatures[entity];
}
