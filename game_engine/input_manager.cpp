#include "input_manager.hpp"
#include <functional>
#include "main.hpp"
#include "transforms.hpp"


void aech::input_manager_t::update(float dt)
{
	if (!holding_u && m_buttons.test(static_cast<size_t>(input_buttons::u)))
	{
		graphics::renderer.gui = !graphics::renderer.gui;
		holding_u = true;
	}
	else if (!m_buttons.test(static_cast<size_t>(input_buttons::u)))
	{
		holding_u = false;
	}

	if (graphics::renderer.gui)
	{
		return;
	}
	
	auto view_matrix = math::get_view_matrix(*m_camera_transform);
	auto right = math::vec3_t {view_matrix[0][0], view_matrix[0][1], view_matrix[0][2]};
	auto up = math::vec3_t {view_matrix[1][0], view_matrix[1][1], view_matrix[1][2]};
	auto forward = math::vec3_t {-view_matrix[2][0], -view_matrix[2][1], -view_matrix[2][2]};

	if (m_buttons.test(static_cast<size_t>(input_buttons::w)))
	{
		m_camera_transform->position += forward * dt * movement_speed;
	}
	else if (m_buttons.test(static_cast<size_t>(input_buttons::s)))
	{
		m_camera_transform->position -= forward * dt * movement_speed;
	}

	if (m_buttons.test(static_cast<size_t>(input_buttons::q)))
	{
		m_camera_transform->position += up * dt * movement_speed;
	}
	else if (m_buttons.test(static_cast<size_t>(input_buttons::e)))
	{
		m_camera_transform->position -= up * dt * movement_speed;
	}

	if (m_buttons.test(static_cast<size_t>(input_buttons::a)))
	{
		m_camera_transform->position -= right * dt * movement_speed;
	}
	else if (m_buttons.test(static_cast<size_t>(input_buttons::d)))
	{
		m_camera_transform->position += right * dt * movement_speed;
	}

	if (offset.first)
	{
		m_camera_transform->rotation.y -= offset.first * mouse_sens;
		m_camera_transform->rotation.y = std::fmod(m_camera_transform->rotation.y, 360.0F);
		offset.first = 0;
	}

	if (offset.second)
	{
		m_camera_transform->rotation.x += offset.second * mouse_sens;
		m_camera_transform->rotation.x = std::clamp(m_camera_transform->rotation.x, -89.0F, 89.0F);
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


void aech::input_manager_t::set_camera_transform(transform_t* camera_transform)
{
	m_camera_transform = camera_transform;
}
