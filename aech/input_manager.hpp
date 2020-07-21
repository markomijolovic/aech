#pragma once

#include "camera.hpp"
#include "event.hpp"
#include <bitset>

namespace aech {
// a class that is responsible for managing the input from the user
class input_manager_t {
public:
    input_manager_t();
    void update(float dt);

    void mouse_listener(events::event_t& event);
    void keyboard_listener(events::event_t& event);
    void set_camera(camera_t* camera);

private:
    inline static float m_movement_speed { 10.0F };
    inline static float m_mouse_sens { 0.1f };
    bool m_holding_o {};

    camera_t* m_camera {};
    std::bitset<32> m_buttons {};
    std::pair<float, float> m_offset {};
};
}
