#include "camera_control_system.hpp"
#include "engine.hpp"
#include "transforms.hpp"
#include "transform.hpp"

extern aech::engine_t engine;

namespace aech
{
	camera_control_system_t::camera_control_system_t()
	{
		auto fn = std::bind(&camera_control_system_t::mouse_listener, this, std::placeholders::_1);
		engine.add_event_listener(events::window::mouse, fn);

		fn = std::bind(&camera_control_system_t::keyboard_listener, this, std::placeholders::_1);
		engine.add_event_listener(events::window::input, fn);
	}

	void camera_control_system_t::update(float dt)
	{
		for (auto& entity : entities)
		{
			auto& transform = engine.get_component<transform_t>(entity);
			auto speed = 2000.0f;

			auto view_matrix = math::get_view_matrix(transform);
			auto right = math::vec3_t{
				view_matrix[0][0],
				view_matrix[0][1],
				view_matrix[0][2]
			};

			auto up = math::vec3_t{
				view_matrix[1][0],
				view_matrix[1][1],
				view_matrix[1][2]
			};

			auto forward = math::vec3_t{ 
				-view_matrix[2][0],
				-view_matrix[2][1],
				-view_matrix[2][2] 
			};

			

			//process keyboard movement
			if (m_buttons.test(static_cast<size_t>(input_buttons::w)))
			{
				transform.position	+= forward* dt * speed;
			}
			else if (m_buttons.test(static_cast<size_t>(input_buttons::s)))
			{
				transform.position -= forward * dt * speed;
			}

			if (m_buttons.test(static_cast<size_t>(input_buttons::q)))
			{
				transform.position += up * dt * speed;
			}
			else if (m_buttons.test(static_cast<size_t>(input_buttons::e)))
			{
				transform.position -= up * dt * speed;
			}

			if (m_buttons.test(static_cast<size_t>(input_buttons::a)))
			{
				transform.position -= right * dt * speed;
			}
			else if (m_buttons.test(static_cast<size_t>(input_buttons::d)))
			{
				transform.position += right * dt * speed;
			}

			//process mouse movement
			if (x_offset)
			{
				transform.rotation.y -= x_offset * 0.01f;
				transform.rotation.y = std::fmod(transform.rotation.y, 360.0f);
				x_offset = 0;
			}

			if (y_offset)
			{
				transform.rotation.x += y_offset * 0.01f;
				transform.rotation.x = std::clamp(transform.rotation.x, -89.0f, 89.0f);
				y_offset = 0;
			}
		}
	}

	void camera_control_system_t::keyboard_listener(events::event_t& event)
	{
		m_buttons = event.get_param<std::bitset<8>>(events::window::params::input);
	}

	void camera_control_system_t::mouse_listener(events::event_t& event)
	{
		auto param = event.get_param<std::pair<float, float>>(events::window::params::mouse);
		x_offset = param.first;
		y_offset = param.second;
	}
}
