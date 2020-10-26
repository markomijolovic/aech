#include "texture_cube.hpp"

#include <algorithm>
#include <glad/glad.h>
#include <ostream>

namespace aech::graphics {
texture_cube_t::texture_cube_t(std::uint32_t                             width,
                               std::uint32_t                             height,
                               texture_types::sized_internal_format sif,
                               texture_types::format                f,
                               texture_types::type                  t,
                               bool                                 mipmap,
                               texture_types::filtering             min,
                               texture_types::filtering             mag)
    : m_width{width}
    , m_height{height}
    , m_sized_internal_format{sif}
    , m_format{f}
    , m_type{t}
    , m_filtering_min{min}
    , m_filtering_mag{mag}
{
    glGenTextures(1, &m_id);

    bind();
    glTexParameteri(static_cast<GLenum>(texture_types::target::cube_map),
                    GL_TEXTURE_MIN_FILTER,
                    static_cast<GLenum>(m_filtering_min));
    glTexParameteri(static_cast<GLenum>(texture_types::target::cube_map),
                    GL_TEXTURE_MAG_FILTER,
                    static_cast<GLenum>(m_filtering_mag));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<GLenum>(m_wrap_s));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast<GLenum>(m_wrap_t));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast<GLenum>(m_wrap_r));

    // TODO: refactor this
    float aniso{};
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, aniso);

    const auto levels = mipmap ? static_cast<std::uint32_t>(floor(log2(std::max(width, height)))) + 1 : 1U;
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, levels, static_cast<GLenum>(m_sized_internal_format), width, height);
    unbind();
}

texture_cube_t &texture_cube_t::operator=(texture_cube_t &&rhs) noexcept
{
    std::swap(m_id, rhs.m_id);
    m_width                 = rhs.m_width;
    m_height                = rhs.m_height;
    m_sized_internal_format = rhs.m_sized_internal_format;
    m_format                = rhs.m_format;
    m_type                  = rhs.m_type;
    m_filtering_mag         = rhs.m_filtering_mag;
    m_filtering_min         = rhs.m_filtering_min;

    return *this;
}

texture_cube_t::texture_cube_t(texture_cube_t &&rhs) noexcept
    : m_id{rhs.m_id}
    , m_width{rhs.m_width}
    , m_height{rhs.m_height}
    , m_sized_internal_format{rhs.m_sized_internal_format}
    , m_format{rhs.m_format}
    , m_type{rhs.m_type}
    , m_filtering_min{rhs.m_filtering_min}
    , m_filtering_mag{rhs.m_filtering_mag}
    , m_mipmap{rhs.m_mipmap}
{
    rhs.m_id = 0;
}

texture_cube_t::~texture_cube_t() noexcept
{
    glDeleteTextures(1, &m_id);
}

std::uint32_t texture_cube_t::id() const
{
    return m_id;
}

void texture_cube_t::bind(std::int32_t unit) const
{
    if (unit >= 0) {
        glActiveTexture(GL_TEXTURE0 + unit);
    }
    glBindTexture(static_cast<GLenum>(texture_types::target::cube_map), m_id);
}

void texture_cube_t::unbind()
{
    glBindTexture(static_cast<GLenum>(texture_types::target::cube_map), 0);
}

void texture_cube_t::generate_mips() const
{
    bind();
    glGenerateMipmap(static_cast<GLenum>(texture_types::target::cube_map));
    unbind();
}

void texture_cube_t::generate_face(std::uint32_t index,
                                   void *   data) const
{
    bind();

    if (data != nullptr) {
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
                        0,
                        0,
                        0,
                        m_width,
                        m_height,
                        static_cast<GLenum>(m_format),
                        static_cast<GLenum>(m_type),
                        data);
    }
    unbind();
    // namespace aech::graphics
}
} // namespace aech::graphics
