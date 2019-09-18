#pragma once

#include "aech_types.hpp"
#include "event.hpp"

#include "system.hpp"


namespace aech
{
	//forward declaration
	class camera_control_system_t : public ecs::system_t
	{
	public:
		void update(float dt);
		camera_control_system_t();

	private:
		std::bitset<8> m_buttons{};
		float          x_offset{};
		float          y_offset{};
		void           mouse_listener(events::event_t& event);
		void           keyboard_listener(events::event_t& event);
	};
} // namespace aech
