#pragma once
#include "event.hpp"

#include "aech_types.hpp"

#include <functional>

#include <list>

#include <unordered_map>


namespace aech::events
{
	// a class that adds listeners for events
	// and is responsible for invoking the registered callbacks
	class event_manager_t
	{
	public:
		void add_listener(event_id_t event_id, const std::function<void(event_t&)>& listener);
		void send_event(event_t& event);
	private:
		std::unordered_map<event_id_t, std::list<std::function<void(event_t&)>>> m_listeners{};
	};
} // namespace aech::events
