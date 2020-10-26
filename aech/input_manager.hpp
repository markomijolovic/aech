#pragma once

#include "camera.hpp"
#include "event.hpp"

#include <bitset>

namespace aech {
// a class that is responsible for managing the input from the user
class input_manager_t {
public:
    input_manager_t();
    auto update(float dt) -> void;

    auto mouse_listener(events::event_t &event) -> void;
    auto keyboard_listener(events::event_t &event) -> void;
    auto set_camera(camera_t *camera) -> void;

private:
    inline static float m_movement_speed{10.0F};
    inline static float m_mouse_sens{0.1F};
    bool                m_holding_o{};

    camera_t *              m_camera{};
    std::bitset<32>         m_buttons{};
    std::pair<float, float> m_offset{};
};
} // namespace aech // namespace aech
