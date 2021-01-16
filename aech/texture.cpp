#include "texture.hpp"
#include <algorithm>
#include <iostream>
#include <cmath>

namespace aech::graphics {
texture_t::texture_t(std::uint32_t width,
    std::uint32_t height,
    texture_types::sized_internal_format sized_internal_format,
    texture_types::format format,
    texture_types::type type,
    bool mipmap,
    texture_types::filtering filtering_min,
    texture_types::filtering filtering_mag,
    void* data)
    : m_width { width }
    , m_height { height }
    , m_sized_internal_format { sized_internal_format }
    , m_format { format }
    , m_type { type }
    , m_filtering_mag { filtering_mag }
    , m_filtering_min { filtering_min }
    , m_mipmap { mipmap }
    , m_data { data }
{
    glGenTextures(1, &m_id);
    bind();

    const auto levels = m_mipmap ? static_cast<std::uint32_t>(floor(log2(std::max(m_width, m_height)))) + 1 : 1U;
    glTexStorage2D(static_cast<GLenum>(texture_types::target::twod),
        levels,
        static_cast<GLenum>(m_sized_internal_format),
        m_width,
        m_height);
    glTexParameteri(static_cast<GLenum>(texture_types::target::twod),
        GL_TEXTURE_MIN_FILTER,
        static_cast<GLenum>(m_filtering_min));
    glTexParameteri(static_cast<GLenum>(texture_types::target::twod),
        GL_TEXTURE_MAG_FILTER,
        static_cast<GLenum>(m_filtering_mag));
    glTexParameteri(static_cast<GLenum>(texture_types::target::twod),
        GL_TEXTURE_WRAP_S,
        static_cast<GLenum>(m_wrap_s));
    glTexParameteri(static_cast<GLenum>(texture_types::target::twod),
        GL_TEXTURE_WRAP_T,
        static_cast<GLenum>(m_wrap_t));

    // TODO: refactor this
    float aniso {};
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, aniso);

    if (m_data != nullptr) {
        glTexSubImage2D(static_cast<GLenum>(texture_types::target::twod),
            0,
            0,
            0,
            m_width,
            m_height,
            static_cast<GLenum>(m_format),
            static_cast<GLenum>(m_type),
            m_data);
    }
    if (m_mipmap) {
        glGenerateMipmap(static_cast<GLenum>(texture_types::target::twod));
    }
    unbind();
}

texture_t::texture_t(texture_t&& rhs) noexcept
    : m_id { rhs.m_id }
    , m_width { rhs.m_width }
    , m_height { rhs.m_height }
    , m_sized_internal_format { rhs.m_sized_internal_format }
    , m_format { rhs.m_format }
    , m_type { rhs.m_type }
    , m_filtering_mag { rhs.m_filtering_mag }
    , m_filtering_min { rhs.m_filtering_min }
    , m_mipmap { rhs.m_mipmap }
    , m_data { rhs.m_data }
{
    rhs.m_id = 0;
}

auto texture_t::operator=(texture_t &&rhs) noexcept -> texture_t&
{
    std::swap(m_id, rhs.m_id);
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_sized_internal_format = rhs.m_sized_internal_format;
    m_format = rhs.m_format;
    m_type = rhs.m_type;
    m_filtering_mag = rhs.m_filtering_mag;
    m_filtering_min = rhs.m_filtering_min;
    m_mipmap = rhs.m_mipmap;
    m_data = rhs.m_data;

    return *this;
}

texture_t::~texture_t() noexcept
{
    glDeleteTextures(1, &m_id);
}

auto texture_t::id() const -> std::uint32_t
{
    return m_id;
}

auto texture_t::bind(std::int32_t unit) const -> void
{
    if (unit >= 0) {
        glActiveTexture(GL_TEXTURE0 + unit);
    }

    glBindTexture(static_cast<GLenum>(texture_types::target::twod), m_id);
}

auto texture_t::unbind() -> void
{
    glBindTexture(static_cast<GLenum>(texture_types::target::twod), 0);
}
} // namespace aech::graphics
