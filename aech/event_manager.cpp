#include "event_manager.hpp"

#include "aech_types.hpp"

namespace aech::events {
auto event_manager_t::add_listener(event_id_t event_id, const std::function<void(event_t &)> &listener) -> void
{
    m_listeners[event_id].push_back(listener);
}

auto event_manager_t::send_event(event_t &event) -> void
{
    const auto type = event.type();
    for (const auto &listener: m_listeners[type]) {
        listener(event);
    }
}
} // namespace aech::events
