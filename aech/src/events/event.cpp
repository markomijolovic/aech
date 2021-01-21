#include "event.hpp"

#include "aech_types.hpp"

namespace aech::events {
event_t::event_t(event_id_t type) noexcept
    : m_type{type}
{
}

auto event_t::type() const noexcept -> event_id_t
{
    return m_type;
}
} // namespace aech::events
