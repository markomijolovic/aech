#pragma once
#include "system.hpp"

namespace aech
{
	class event_t;

	class player_control_system_t : public system_t
	{
	public:
		void init();
		void update(float dt);

	private:
		std::bitset<8> m_buttons;
		void input_listener(event_t& event);
	};
}
