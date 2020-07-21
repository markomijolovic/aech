#include "input_manager.hpp"
#include "main.hpp"
#include <functional>

void aech::input_manager_t::update(float dt)
{
    if (!m_holding_o && m_buttons.test(static_cast<size_t>(input_buttons::o))) {
        graphics::renderer.set_options(!graphics::renderer.options());
        m_holding_o = true;
    } else if (!m_buttons.test(static_cast<size_t>(input_buttons::o))) {
        m_holding_o = false;
    }

    if (graphics::renderer.options()) {
        return;
    }

    auto view_matrix = m_camera->view_matrix();
    const auto right = math::vec3_t { view_matrix[0][0], view_matrix[0][1], view_matrix[0][2] };
    const auto up = math::vec3_t { view_matrix[1][0], view_matrix[1][1], view_matrix[1][2] };
    const auto forward = math::vec3_t { -view_matrix[2][0], -view_matrix[2][1], -view_matrix[2][2] };

    math::vec3_t trans {};
    if (m_buttons.test(static_cast<size_t>(input_buttons::w))) {
        trans += forward * dt * m_movement_speed;
    }
    if (m_buttons.test(static_cast<size_t>(input_buttons::s))) {
        trans -= forward * dt * m_movement_speed;
    }

    if (m_buttons.test(static_cast<size_t>(input_buttons::q))) {
        trans += up * dt * m_movement_speed;
    }
    if (m_buttons.test(static_cast<size_t>(input_buttons::e))) {
        trans -= up * dt * m_movement_speed;
    }

    if (m_buttons.test(static_cast<size_t>(input_buttons::a))) {
        trans -= right * dt * m_movement_speed;
    }
    if (m_buttons.test(static_cast<size_t>(input_buttons::d))) {
        trans += right * dt * m_movement_speed;
    }
    m_camera->translate(trans);

    if (m_offset.first != 0.0F) {
        m_camera->rotate_y(-m_offset.first * m_mouse_sens);
        m_offset.first = 0;
    }

    if (m_offset.second != 0.0F) {
        m_camera->rotate_x(m_offset.second * m_mouse_sens);
        m_offset.second = 0;
    }
}

aech::input_manager_t::input_manager_t()
{
    const auto ml = std::bind(&input_manager_t::mouse_listener, this, std::placeholders::_1);
    engine.add_event_listener(events::window::mouse, ml);

    const auto kl = std::bind(&input_manager_t::keyboard_listener, this, std::placeholders::_1);
    engine.add_event_listener(events::window::keyboard, kl);
}

void aech::input_manager_t::mouse_listener(events::event_t& event)
{
    m_offset = event.get_param<std::pair<float, float>>(events::window::params::mouse);
}

void aech::input_manager_t::keyboard_listener(events::event_t& event)
{
    m_buttons = event.get_param<std::bitset<32>>(events::window::params::keyboard);
}

void aech::input_manager_t::set_camera(camera_t* camera)
{
    m_camera = camera;
}
