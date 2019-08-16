#include "event_manager.hpp"

namespace aech
{
	void event_manager_t::add_listener(event_id_t event_id, const std::function<void(event_t&)>& listener)
	{
		listeners[event_id].push_back(listener);
	}

	void event_manager_t::send_event(event_t& event)
	{
		auto type = event.get_type();
		for (const auto& listener : listeners[type])
			listener(event);
	}

	void event_manager_t::send_event(event_id_t event_id)
	{
		event_t event{ event_id };

		for (const auto& listener : listeners[event_id])
			listener(event);
	}
}
