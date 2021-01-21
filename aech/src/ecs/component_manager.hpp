#pragma once

#include "component_array.hpp"

#include <memory>
#include <typeindex>
#include <unordered_map>

namespace aech::ecs {
class component_manager_t {
public:
    template <typename T>
    auto register_component() noexcept -> void
    {
        // NOTE: cv-qualifiers are ignored by std::type_info
        const auto &type_info  = typeid(std::remove_reference_t<T>);
        const auto &type_index = std::type_index{type_info};

        m_component_types.insert({type_index, m_next_component_type++});
        m_component_arrays.insert({type_index, std::make_shared<component_array_t<T>>()});
    }

    template <typename T>
    [[nodiscard]] auto get_component_type() noexcept -> component_type_t
    {
        const auto &type_info  = typeid(std::remove_reference_t<T>);
        const auto &type_index = std::type_index{type_info};

        return m_component_types[type_index];
    }

    // adds a component to the array for the specified entity
    template <typename T>
    auto add_component(entity_t entity, T &&component) noexcept -> void
    {
        get_component_array<T>()->insert_data(entity, std::forward<T>(component));
    }

    template <typename T>
    [[nodiscard]] auto get_component(entity_t entity) noexcept -> T &
    {
        return get_component_array<T>()->get_data(entity);
    }

    template <typename T>
    auto remove_component(entity_t entity) noexcept -> void
    {
        get_component_array<T>()->remove_data(entity);
    }

    template <typename T>
    [[nodiscard]] auto has_component(entity_t entity) noexcept -> bool
    {
        return get_component_array<T>()->has_entity(entity);
    }

    auto entity_destroyed(entity_t entity) noexcept -> void;

private:
    std::unordered_map<std::type_index, component_type_t>                   m_component_types{};
    std::unordered_map<std::type_index, std::shared_ptr<component_array_i>> m_component_arrays{};
    component_type_t                                                        m_next_component_type{};

    // helper function to get pointer to the component_array_t of type T
    template <typename T>
    [[nodiscard]] auto get_component_array() noexcept -> std::shared_ptr<component_array_t<T>>
    {
        const auto &type_info  = typeid(std::remove_reference_t<T>);
        const auto &type_index = std::type_index{type_info};

        return std::static_pointer_cast<component_array_t<T>>(m_component_arrays[type_index]);
    }
};
} // namespace aech
