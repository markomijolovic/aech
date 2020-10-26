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

    [[nodiscard]] auto should_close() const -> bool;
    auto               begin_frame() const -> void;
    auto               end_frame() const -> void;

    [[nodiscard]] auto buttons() const -> std::bitset<32>;
    [[nodiscard]] auto first_mouse() const -> bool;
    [[nodiscard]] auto x() const -> float;
    [[nodiscard]] auto y() const -> float;

    auto set_button(input_buttons button) -> void;
    auto reset_button(input_buttons button) -> void;
    auto set_first_mouse(bool f) -> void;
    auto set_x(float x) -> void;
    auto set_y(float y) -> void;

    [[nodiscard]] auto width() const -> std::uint32_t;
    [[nodiscard]] auto height() const -> std::uint32_t;

private:
    bool  m_first_mouse{true};
    float m_last_x{};
    float m_last_y{};

    // 2.4 aspect ratio
    std::uint32_t m_screen_width  = 1920;
    std::uint32_t m_screen_height = 800;

    std::bitset<32> m_buttons{};
    GLFWwindow *    m_window{};
};
} // namespace aech::graphics
