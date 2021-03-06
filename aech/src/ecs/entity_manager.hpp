#pragma once

#include "aech_types.hpp"

#include <array>
#include <queue>

namespace aech::ecs {
// a class that manages individual entity IDs
class entity_manager_t {
public:
    entity_manager_t() noexcept;
    [[nodiscard]] auto create_entity() noexcept -> entity_t;
    auto               destroy_entity(entity_t entity) noexcept -> void;
    auto               set_signature(entity_t entity, signature_t signature) noexcept -> void;
    [[nodiscard]] auto get_signature(entity_t entity) const noexcept -> signature_t;

private:
    std::queue<entity_t> m_available_entities{};

    // an array of signatures
    // index is an entity id
    // value is that entity's signature -
    // i.e. what components it has
    std::array<signature_t, max_entities + 1> m_signatures{};

    std::uint32_t m_living_entities_count{};
};
} // namespace aech
