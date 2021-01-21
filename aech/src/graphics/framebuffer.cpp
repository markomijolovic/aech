#include "framebuffer.hpp"

#include <cassert>

namespace aech::graphics {
auto framebuffer_t::id() const noexcept -> std::uint32_t
{
    return m_id;
}

auto framebuffer_t::width() const noexcept -> std::uint32_t
{
    return m_width;
}

auto framebuffer_t::height() const noexcept -> std::uint32_t
{
    return m_height;
}

auto framebuffer_t::colour_attachments() const noexcept -> const std::vector<texture_t> &
{
    return m_colour_attachments;
}

auto framebuffer_t::depth_and_stencil() const noexcept -> const texture_t *
{
    return m_depth_and_stencil_texture.get();
}

auto framebuffer_t::operator=(framebuffer_t &&rhs) noexcept -> framebuffer_t &
{
    std::swap(m_id, rhs.m_id);
    m_width                     = rhs.m_width;
    m_height                    = rhs.m_height;
    m_depth_and_stencil_texture = std::move(rhs.m_depth_and_stencil_texture);
    m_colour_attachments        = std::move(rhs.m_colour_attachments);
    m_mipmap                    = rhs.m_mipmap;

    return *this;
}

framebuffer_t::~framebuffer_t() noexcept
{
    // 0 is silently ignored
    glDeleteFramebuffers(1, &m_id);
}

framebuffer_t::framebuffer_t(std::uint32_t                        width,
                             std::uint32_t                        height,
                             std::uint32_t                        num_colour_attachments,
                             bool                                 depth,
                             texture_types::sized_internal_format sized_internal_format,
                             texture_types::format                format,
                             texture_types::type                  type) noexcept
    : m_width{width}
    , m_height{height}
{
    glGenFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    for (std::uint32_t i = 0; i < num_colour_attachments; i++) {
        m_colour_attachments.emplace_back(width, height, sized_internal_format, format, type, m_mipmap);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + i,
                               GL_TEXTURE_2D,
                               m_colour_attachments.back().id(),
                               0);
    }

    if (depth) {
        m_depth_and_stencil_texture = std::make_unique<texture_t>(width,
                                                                  height,
                                                                  texture_types::sized_internal_format::depth32f,
                                                                  texture_types::format::depth,
                                                                  texture_types::type::floating_point,
                                                                  false);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_2D,
                               m_depth_and_stencil_texture->id(),
                               0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto framebuffer_t::bind() const noexcept -> void
{
    assert(m_id);

    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

auto framebuffer_t::unbind() noexcept -> void
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
} // namespace aech::graphics
