#pragma once

#include "texture.hpp"
#include <memory>
#include <vector>

namespace aech::graphics {
class framebuffer_t {
public:
    // make unordered_map happy
    framebuffer_t() = default;
    framebuffer_t(
        uint32_t width,
        uint32_t height,
        uint32_t num_colour_attachments,
        bool depth,
        texture_types::sized_internal_format sized_internal_format,
        texture_types::format format,
        texture_types::type type);
    framebuffer_t(const framebuffer_t& rhs) = delete;
    framebuffer_t(framebuffer_t&& rhs) = delete;
    framebuffer_t& operator=(const framebuffer_t& rhs) = delete;
    framebuffer_t& operator=(framebuffer_t&& rhs) noexcept;
    ~framebuffer_t() noexcept;

    [[nodiscard]] uint32_t id() const;
    [[nodiscard]] uint32_t width() const;
    [[nodiscard]] uint32_t height() const;
    void bind() const;
    static void unbind();
    [[nodiscard]] const std::vector<texture_t>& colour_attachments() const;
    [[nodiscard]] const texture_t* depth_and_stencil() const;

private:
    uint32_t m_id {};
    uint32_t m_width {};
    uint32_t m_height {};
    std::unique_ptr<texture_t> m_depth_and_stencil_texture {};
    std::vector<texture_t> m_colour_attachments {};
    bool m_mipmap {};
};
} // namespace aech::graphics
