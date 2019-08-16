#pragma once

#include "system.hpp"
#include "event.hpp"

namespace aech
{
	//forward declaration
	class camera_control_system_t : public system_t
	{
	public:
		void init();
		void update(float dt);

	private:
		std::bitset<8> m_buttons{};
		void input_listener(event_t& event);
	};
}
