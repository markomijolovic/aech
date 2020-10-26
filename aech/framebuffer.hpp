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
        std::uint32_t                             width,
        std::uint32_t                             height,
        std::uint32_t                             num_colour_attachments,
        bool                                 depth,
        texture_types::sized_internal_format sized_internal_format,
        texture_types::format                format,
        texture_types::type                  type);
    framebuffer_t(const framebuffer_t &rhs) = delete;
    framebuffer_t(framebuffer_t &&rhs)      = delete;
    auto operator=(const framebuffer_t &rhs) -> framebuffer_t & = delete;
    auto operator                                               =(framebuffer_t &&rhs) noexcept -> framebuffer_t &;
    ~framebuffer_t() noexcept;

    [[nodiscard]] auto id() const -> std::uint32_t;
    [[nodiscard]] auto width() const -> std::uint32_t;
    [[nodiscard]] auto height() const -> std::uint32_t;
    auto               bind() const -> void;
    static auto        unbind() -> void;
    [[nodiscard]] auto colour_attachments() const -> const std::vector<texture_t> &;
    [[nodiscard]] auto depth_and_stencil() const -> const texture_t *;

private:
    std::uint32_t                   m_id{};
    std::uint32_t                   m_width{};
    std::uint32_t                   m_height{};
    std::unique_ptr<texture_t> m_depth_and_stencil_texture{};
    std::vector<texture_t>     m_colour_attachments{};
    bool                       m_mipmap{};
};
} // namespace aech::graphics
