#include "system_manager.hpp"

namespace aech::ecs {
auto system_manager_t::entity_destroyed(entity_t entity) noexcept -> void
{
    for (const auto &el: m_systems) {
        el.second->entities.erase(entity);
    }
}

auto system_manager_t::entity_signature_changed(entity_t entity, signature_t entity_signature) noexcept -> void
{
    for (const auto &el: m_systems) {
        const auto &type   = el.first;
        const auto &system = el.second;

        if (const auto &system_signature = m_signatures[type]; (entity_signature & system_signature) == system_signature) {
            system->entities.insert(entity);
        } else {
            system->entities.erase(entity);
        }
    }
}
} // namespace aech
