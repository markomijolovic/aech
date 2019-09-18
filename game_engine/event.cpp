#include "event.hpp"
#include "aech_types.hpp"

namespace aech::events
{
	event_t::event_t(event_id_t type)
		: m_type{ type }
	{
	}

	event_id_t event_t::type() const
	{
		return m_type;
	}
}
