#pragma once
#include <bitset>
#include "transform.hpp"
#include <memory>
#include "camera_frustum.hpp"
#include "camera.hpp"
#include "event.hpp"

namespace aech
{
	class input_manager_t
	{
	public:
		input_manager_t();
		void update(float dt);

		void mouse_listener(events::event_t& event);
		void keyboard_listener(events::event_t& event);
		void set_camera(camera_t* camera);

	private:
		inline static float movement_speed{500.0F};
		inline static float mouse_sens{0.1f};
		bool                holding_o{};

		camera_t*               m_camera{};
		std::bitset<32>         m_buttons{};
		std::pair<float, float> offset{};
	};
}
