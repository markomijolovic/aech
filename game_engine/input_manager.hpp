#pragma once
#include <bitset>
#include "transform.hpp"
#include <memory>

namespace aech
{
	namespace events {
		class event_t;
	}

	class input_manager_t
	{
	public:
		input_manager_t();
		void update(float dt);

		void mouse_listener(events::event_t& event);
		void keyboard_listener(events::event_t& event);
		void set_camera_transform(transform_t* camera_transform);
		
	private:
		inline static float movement_speed{2000.0f};
		inline static float mouse_sens{0.1f};
		bool holding_u {};
		
		transform_t *m_camera_transform{};
		std::bitset<32> m_buttons{};
		std::pair<float, float> offset{};
	};
}
