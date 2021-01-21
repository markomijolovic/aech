#pragma once

#include "texture_cube.hpp"

namespace aech::graphics {
// TODO(Marko): should you be able to allocate a framebuffer on the stack
class framebuffer_cube_t {
public:
    framebuffer_cube_t() = default;
    framebuffer_cube_t(
        texture_cube_t *texture,
        std::uint32_t   width,
        std::uint32_t   height);
    framebuffer_cube_t(const framebuffer_cube_t &rhs) = delete;
    framebuffer_cube_t(framebuffer_cube_t &&rhs)      = delete;
    auto operator=(const framebuffer_cube_t &rhs) -> framebuffer_cube_t & = delete;
    auto operator                                                         =(framebuffer_cube_t &&rhs) noexcept -> framebuffer_cube_t &;
    ~framebuffer_cube_t() noexcept;

    auto        attach(std::uint32_t i, std::uint32_t miplevel = 0) const noexcept -> void;
    auto        bind() const noexcept -> void;
    static auto unbind() noexcept -> void;

    [[nodiscard]] auto width() const noexcept -> std::uint32_t;
    [[nodiscard]] auto height() const noexcept -> std::uint32_t;
    [[nodiscard]] auto texture() const noexcept -> texture_cube_t *;

private:
    std::uint32_t id{};
    std::uint32_t rbo_id{};
    // NOTE: this is a NON-OWNING pointer!!!
    // it can not be a reference because we require framebuffers to be default constructible and we can not determine the proper object to refer to at construction time
    texture_cube_t *m_texture{};
    std::uint32_t   m_width{};
    std::uint32_t   m_height{};
};
} // namespace aech::graphics
