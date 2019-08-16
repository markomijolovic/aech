#include "camera_control_system.hpp"
#include "engine.hpp"
#include "components.hpp"

extern aech::engine_t engine;

namespace aech
{
	void camera_control_system_t::init()
	{
		auto fn = std::bind(&camera_control_system_t::input_listener, this, std::placeholders::_1);
		engine.add_event_listener(events::window::input, fn);
	}

	void camera_control_system_t::update(float dt)
	{
		for (auto &entity: m_entities)
		{
			auto& transform = engine.get_component<transform_t>(entity);
			auto speed = 20.0f;

			if (m_buttons.test(static_cast<size_t>(input_buttons::w)))
			{
				transform.position.z -= dt * speed;
			}
			else if (m_buttons.test(static_cast<size_t>(input_buttons::s)))
			{
				transform.position.z += dt * speed;
			}

			if (m_buttons.test(static_cast<size_t>(input_buttons::q)))
			{
				transform.position.y -= dt * speed;
			}
			else if (m_buttons.test(static_cast<size_t>(input_buttons::e)))
			{
				transform.position.y += dt * speed;
			}

			if (m_buttons.test(static_cast<size_t>(input_buttons::a)))
			{
				transform.position.x -= dt * speed;
			}
			else if (m_buttons.test(static_cast<size_t>(input_buttons::d)))
			{
				transform.position.x += dt * speed;
			}

		}
	}

	void camera_control_system_t::input_listener(event_t& event)
	{
		m_buttons = event.get_param<std::bitset<8>>(events::window::params::input);
	}
}
