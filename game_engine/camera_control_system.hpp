#pragma once

#include "system.hpp"
#include "event.hpp"

namespace aech
{
	//forward declaration
	class camera_control_system_t : public system_t
	{
	public:
		void update(float dt);
		camera_control_system_t();

	private:
		std::bitset<8> m_buttons{};
		float x_offset{};
		float y_offset{};
		void mouse_listener(event_t& event);
		void keyboard_listener(event_t& event);
	};
}
