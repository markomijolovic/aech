#pragma once

#include "aech_types.hpp"

struct GLFWwindow;

namespace aech::graphics {
// a class that manages the application window
class window_manager_t {
public:
    // initializes GLFW and ImGui
    window_manager_t() noexcept;

    window_manager_t(const window_manager_t &) = delete;
    window_manager_t(window_manager_t &&)      = default;

    auto operator=(const window_manager_t &) -> window_manager_t & = delete;
    auto operator=(window_manager_t &&) -> window_manager_t & = default;
    // shuts GLFW and ImGui down
    ~window_manager_t() noexcept;

    [[nodiscard]] auto should_close() const noexcept -> bool;
    auto               begin_frame() const noexcept -> void;
    auto               end_frame() const noexcept -> void;

    [[nodiscard]] auto buttons() const noexcept -> std::bitset<32>;
    [[nodiscard]] auto first_mouse() const noexcept -> bool;
    [[nodiscard]] auto x() const noexcept -> float;
    [[nodiscard]] auto y() const noexcept -> float;

    auto set_button(input_buttons button) noexcept -> void;
    auto reset_button(input_buttons button) noexcept -> void;
    auto set_first_mouse(bool f) noexcept -> void;
    auto set_x(float x) noexcept -> void;
    auto set_y(float y) noexcept -> void;

    [[nodiscard]] auto width() const noexcept -> std::uint32_t;
    [[nodiscard]] auto height() const noexcept -> std::uint32_t;

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
