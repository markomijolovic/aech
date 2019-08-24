#pragma once
#include "system.hpp"

namespace aech
{
	class event_t;

	class player_control_system_t : public system_t
	{
	public:
		void update(float dt);
		player_control_system_t();

	private:
		std::bitset<8> m_buttons;
		void input_listener(event_t& event);
	};
}
