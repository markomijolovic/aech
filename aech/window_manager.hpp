#pragma once

#include "aech_types.hpp"

struct GLFWwindow;

namespace aech::graphics {
// a class that manages the application window
class window_manager_t {
public:
    // initializes GLFW and ImGui
    window_manager_t();

    // shuts GLFW and ImGui down
    ~window_manager_t();

    [[nodiscard]] bool should_close() const;
    void begin_frame() const;
    void end_frame() const;

    [[nodiscard]] std::bitset<32> buttons() const;
    [[nodiscard]] bool first_mouse() const;
    [[nodiscard]] float x() const;
    [[nodiscard]] float y() const;

    void set_button(input_buttons button);
    void reset_button(input_buttons button);
    void set_first_mouse(bool f);
    void set_x(float x);
    void set_y(float y);

    [[nodiscard]] uint32_t width() const;
    [[nodiscard]] uint32_t height() const;

private:
    bool m_first_mouse { true };
    float m_last_x {};
    float m_last_y {};

    // 2.4 aspect ratio
    uint32_t m_screen_width = 1920;
    uint32_t m_screen_height = 800;

    std::bitset<32> m_buttons {};
    GLFWwindow* m_window {};
};
} // namespace aech::graphics
