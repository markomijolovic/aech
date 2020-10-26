#include "framebuffer_cube.hpp"

aech::graphics::framebuffer_cube_t::~framebuffer_cube_t() noexcept
{
    glDeleteRenderbuffers(1, &rbo_id);
    glDeleteFramebuffers(1, &id);
}

auto aech::graphics::framebuffer_cube_t::operator=(framebuffer_cube_t &&rhs) noexcept -> aech::graphics::framebuffer_cube_t &
{
    std::swap(rbo_id, rhs.rbo_id);
    std::swap(id, rhs.id);
    m_width   = rhs.m_width;
    m_height  = rhs.m_height;
    m_texture = rhs.m_texture;

    return *this;
}

aech::graphics::framebuffer_cube_t::framebuffer_cube_t(texture_cube_t *texture, std::uint32_t width, std::uint32_t height)
    : m_texture{texture}
    , m_width{width}
    , m_height{height}
{
    glGenFramebuffers(1, &id);
    glGenRenderbuffers(1, &rbo_id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_id);
}

auto aech::graphics::framebuffer_cube_t::attach(std::uint32_t i, std::uint32_t miplevel) const -> void
{
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                           m_texture->id(),
                           miplevel);
}

auto aech::graphics::framebuffer_cube_t::bind() const -> void
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

auto aech::graphics::framebuffer_cube_t::texture() const -> aech::graphics::texture_cube_t *
{
    return m_texture;
}

auto aech::graphics::framebuffer_cube_t::unbind() -> void
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto aech::graphics::framebuffer_cube_t::width() const -> std::uint32_t
{
    return m_width;
}

auto aech::graphics::framebuffer_cube_t::height() const -> std::uint32_t
{
    return m_height;
}
