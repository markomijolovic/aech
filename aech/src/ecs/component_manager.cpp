#include "component_manager.hpp"

namespace aech::ecs {
auto component_manager_t::entity_destroyed(entity_t entity) noexcept -> void
{
    for (const auto &el: m_component_arrays) {
        el.second->entity_destroyed(entity);
    }
}
} // namespace aech::ecs
