#include "event.hpp"

#include "aech_types.hpp"

namespace aech::events {
event_t::event_t(event_id_t type)
    : m_type{type}
{
}

auto event_t::type() const -> event_id_t
{
    return m_type;
}
} // namespace aech::events
