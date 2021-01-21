#pragma once

#include "component_manager.hpp"
#include "event_manager.hpp"
#include "system_manager.hpp"

#include <memory>

namespace aech {
// a class that bundles the functionality provided by the ECS architecture together
class engine_t {
public:
    [[nodiscard]] auto create_entity() noexcept -> entity_t;
    [[nodiscard]] auto root_node() const noexcept -> entity_t;
    auto               destroy_entity(entity_t entity) noexcept -> void;
    auto               set_root_node(entity_t root_node) noexcept -> void;

    engine_t() noexcept;

    template <typename T>
    auto register_component() noexcept -> void
    {
        m_component_manager.register_component<T>();
    }

    template <typename T>
    auto add_component(entity_t entity, T &&component) noexcept -> void
    {
        m_component_manager.add_component<T>(entity, std::forward<T>(component));

        auto signature = m_entity_manager.get_signature(entity);
        signature.set(m_component_manager.get_component_type<T>(), true);

        m_entity_manager.set_signature(entity, signature);
        m_system_manager.entity_signature_changed(entity, signature);
    }

    template <typename T>
    [[nodiscard]] auto has_component(entity_t entity) noexcept -> bool
    {
        return m_component_manager.has_component<T>(entity);
    }

    template <typename T>
    auto remove_component(entity_t entity) noexcept -> void
    {
        m_component_manager.remove_component<T>(entity);

        auto signature = m_entity_manager.get_signature(entity);
        signature.set(m_component_manager.get_component_type<T>(), false);
        m_entity_manager.set_signature(entity, signature);

        m_system_manager.entity_signature_changed(entity, signature);
    }

    template <typename T>
    [[nodiscard]] auto get_component(entity_t entity) noexcept -> T &
    {
        return m_component_manager.get_component<T>(entity);
    }

    template <typename T>
    [[nodiscard]] auto get_component_type() noexcept -> component_type_t
    {
        return m_component_manager.get_component_type<T>();
    }

    template <typename T, typename... Args>
    [[nodiscard]] auto register_system(Args &&... args) noexcept -> std::shared_ptr<T>
    {
        return m_system_manager.register_system<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    auto set_system_signature(signature_t signature) noexcept -> void
    {
        m_system_manager.set_signature<T>(signature);
    }

    auto add_event_listener(event_id_t event_id, const std::function<void(events::event_t &)> &listener) noexcept -> void;
    auto send_event(events::event_t &event) noexcept -> void;

private:
    entity_t                 m_root_node{};
    ecs::component_manager_t m_component_manager{};
    ecs::entity_manager_t    m_entity_manager{};
    ecs::system_manager_t    m_system_manager{};
    events::event_manager_t  m_event_manager{};
};
} // namespace aech
