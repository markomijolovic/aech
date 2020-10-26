#pragma once

#include "entity_manager.hpp"

#include <unordered_map>

namespace aech::ecs {
class component_array_i {
public:
    virtual ~component_array_i()                   = default;
    virtual void entity_destroyed(entity_t entity) = 0;
};

template <typename T>
class component_array_t final : public component_array_i {
public:
    auto insert_data(entity_t entity, T &&component) -> void
    {
        auto index                = m_size;
        m_entity_to_index[entity] = index;
        m_index_to_entity[index]  = entity;
        m_component_array[index]  = std::forward<T>(component);
        m_size++;
    }

    auto has_entity(entity_t entity) -> bool
    {
        return m_entity_to_index.find(entity) != std::end(m_entity_to_index);
    }

    auto remove_data(entity_t entity) -> void
    {
        auto index               = m_entity_to_index[entity];
        auto index_of_last       = m_size - 1;
        m_component_array[index] = std::move(m_component_array[index_of_last]);

        const auto entity_of_last         = m_index_to_entity[index_of_last];
        m_entity_to_index[entity_of_last] = index;
        m_index_to_entity[index]          = entity_of_last;

        m_entity_to_index.erase(entity);
        m_index_to_entity.erase(index_of_last);
        m_size--;
    }

    auto get_data(entity_t entity) -> T &
    {
        auto index = m_entity_to_index[entity];
        return m_component_array[index];
    }

    void entity_destroyed(entity_t entity) override
    {
        const auto it = m_entity_to_index.find(entity);
        if (it != std::end(m_entity_to_index)) {
            remove_data(entity);
        }
    }

private:
    std::array<std::remove_reference_t<T>, max_entities> m_component_array{};

    // maps entity ID to array index
    std::unordered_map<entity_t, std::size_t> m_entity_to_index{};

    // maps array index to entity ID
    std::unordered_map<std::size_t, entity_t> m_index_to_entity{};

    std::size_t m_size{};
};
} // namespace aech
