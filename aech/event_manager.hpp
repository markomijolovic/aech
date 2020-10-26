#pragma once

#include "aech_types.hpp"
#include "event.hpp"

#include <functional>
#include <list>
#include <unordered_map>

namespace aech::events {
// a class that adds listeners for events
// and is responsible for invoking the registered callbacks
class event_manager_t {
public:
    auto add_listener(event_id_t event_id, const std::function<void(event_t &)> &listener) -> void;
    auto send_event(event_t &event) -> void;

private:
    std::unordered_map<event_id_t, std::list<std::function<void(event_t &)>>> m_listeners{};
};
} // namespace aech::events
