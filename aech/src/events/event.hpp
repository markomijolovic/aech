#pragma once

#include "aech_types.hpp"

#include <any>
#include <unordered_map>

namespace aech::events {
// a class that represents an event
// an event is composed of an ID and corresponding parameters
class event_t {
public:
    explicit event_t(event_id_t type) noexcept;
    [[nodiscard]] auto type() const noexcept -> event_id_t;

    template <typename T>
    auto set_param(param_id_t id, T &&value) noexcept -> void
    {
        m_data[id] = std::forward<T>(value);
    }

    template <typename T>
    [[nodiscard]] auto get_param(param_id_t id) noexcept -> T
    {
        return std::any_cast<T>(m_data[id]);
    }

private:
    event_id_t                               m_type{};
    std::unordered_map<param_id_t, std::any> m_data{};
};
} // namespace aech::events
