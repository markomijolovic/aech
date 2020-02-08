#include "input_manager.hpp"
#include <functional>
#include "main.hpp"
#include "transforms.hpp"


void aech::input_manager_t::update(float dt)
{
	if (!holding_o && m_buttons.test(static_cast<size_t>(input_buttons::o)))
	{
		graphics::renderer.set_options(!graphics::renderer.options());
		holding_o = true;
	}
	else if (!m_buttons.test(static_cast<size_t>(input_buttons::o)))
	{
		holding_o = false;
	}

	if (graphics::renderer.options())
	{
		return;
	}

	auto view_matrix = m_camera->view_matrix();
	auto right       = math::vec3_t{view_matrix[0][0], view_matrix[0][1], view_matrix[0][2]};
	auto up          = math::vec3_t{view_matrix[1][0], view_matrix[1][1], view_matrix[1][2]};
	auto forward     = math::vec3_t{-view_matrix[2][0], -view_matrix[2][1], -view_matrix[2][2]};

	math::vec3_t trans{};
	if (m_buttons.test(static_cast<size_t>(input_buttons::w)))
	{
		trans += forward * dt * movement_speed;
	}
	if (m_buttons.test(static_cast<size_t>(input_buttons::s)))
	{
		trans -= forward * dt * movement_speed;
	}

	if (m_buttons.test(static_cast<size_t>(input_buttons::q)))
	{
		trans += up * dt * movement_speed;
	}
	if (m_buttons.test(static_cast<size_t>(input_buttons::e)))
	{
		trans -= up * dt * movement_speed;
	}

	if (m_buttons.test(static_cast<size_t>(input_buttons::a)))
	{
		trans -= right * dt * movement_speed;
	}
	if (m_buttons.test(static_cast<size_t>(input_buttons::d)))
	{
		trans += right * dt * movement_speed;
	}
	m_camera->translate(trans);

	if (offset.first)
	{
		m_camera->rotate_y(-offset.first * mouse_sens);
		offset.first = 0;
	}

	if (offset.second)
	{
		m_camera->rotate_x(offset.second * mouse_sens);
		offset.second = 0;
	}
}

aech::input_manager_t::input_manager_t()
{
	auto ml = std::bind(&input_manager_t::mouse_listener, this, std::placeholders::_1);
	engine.add_event_listener(events::window::mouse, ml);

	auto kl = std::bind(&input_manager_t::keyboard_listener, this, std::placeholders::_1);
	engine.add_event_listener(events::window::keyboard, kl);
}

void aech::input_manager_t::mouse_listener(events::event_t& event)
{
	offset = event.get_param<std::pair<float, float>>(events::window::params::mouse);
}

void aech::input_manager_t::keyboard_listener(events::event_t& event)
{
	m_buttons = event.get_param<std::bitset<32>>(events::window::params::keyboard);
}

void aech::input_manager_t::set_camera(camera_t* camera)
{
	m_camera = camera;
}