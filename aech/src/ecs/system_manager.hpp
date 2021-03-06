#pragma once

#include "aech_types.hpp"
#include "system.hpp"

#include <memory>
#include <typeindex>
#include <unordered_map>

namespace aech::ecs {
class system_manager_t {
public:
    template <typename T, typename... Args>
    [[nodiscard]] auto register_system(Args &&... args) noexcept -> std::shared_ptr<T>
    {
        const auto &type_info  = typeid(std::remove_reference_t<T>);
        const auto &type_index = std::type_index{type_info};

        auto system = std::make_shared<T>(std::forward<Args>(args)...);

        m_systems.insert({type_index, system});

        return system;
    }

    template <typename T>
    auto set_signature(signature_t signature) noexcept -> void
    {
        const auto &type_info  = typeid(std::remove_reference_t<T>);
        const auto &type_index = std::type_index{type_info};

        m_signatures.insert({type_index, signature});
    }

    auto entity_destroyed(entity_t entity) noexcept -> void;

    auto entity_signature_changed(entity_t entity, signature_t entity_signature) noexcept -> void;

private:
    std::unordered_map<std::type_index, signature_t>               m_signatures{};
    std::unordered_map<std::type_index, std::shared_ptr<system_t>> m_systems{};
};
} // namespace aech
