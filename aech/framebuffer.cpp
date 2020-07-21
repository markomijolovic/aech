#include "framebuffer.hpp"

namespace aech::graphics {
uint32_t framebuffer_t::id() const
{
    return m_id;
}

uint32_t framebuffer_t::width() const
{
    return m_width;
}

uint32_t framebuffer_t::height() const
{
    return m_height;
}

const std::vector<texture_t>& framebuffer_t::colour_attachments() const
{
    return m_colour_attachments;
}

const texture_t* framebuffer_t::depth_and_stencil() const
{
    return m_depth_and_stencil_texture.get();
}

framebuffer_t& framebuffer_t::operator=(framebuffer_t&& rhs) noexcept
{
    std::swap(m_id, rhs.m_id);
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_depth_and_stencil_texture = std::move(rhs.m_depth_and_stencil_texture);
    m_colour_attachments = std::move(rhs.m_colour_attachments);
    m_mipmap = rhs.m_mipmap;

    return *this;
}

framebuffer_t::~framebuffer_t() noexcept
{
    glDeleteFramebuffers(1, &m_id);
}

framebuffer_t::framebuffer_t(uint32_t width,
    uint32_t height,
    uint32_t num_colour_attachments,
    bool depth,
    texture_types::sized_internal_format sized_internal_format,
    texture_types::format format,
    texture_types::type type)
    : m_width { width }
    , m_height { height }
{
    glGenFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    for (uint32_t i = 0; i < num_colour_attachments; i++) {
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

void framebuffer_t::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void framebuffer_t::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
} // namespace aech::graphics
