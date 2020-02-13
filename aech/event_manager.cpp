#include "event_manager.hpp"
#include "aech_types.hpp"

namespace aech::events
{
	void event_manager_t::add_listener(event_id_t event_id, const std::function<void(event_t&)>& listener)
	{
		listeners[event_id].push_back(listener);
	}

	void event_manager_t::send_event(event_t& event)
	{
		const auto type = event.type();
		for (const auto& listener : listeners[type])
		{
			listener(event);
		}
	}
} // namespace aech::events
