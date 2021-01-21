#pragma once

#include "aech_types.hpp"
#include "texture_types.hpp"

namespace aech::graphics {
// a class that represents a texture
class texture_t {
public:
    // make unordered_map happy
    texture_t() = default;

    // creates an immutable texture
    texture_t(std::uint32_t                        width,
              std::uint32_t                        height,
              texture_types::sized_internal_format sized_internal_format = texture_types::sized_internal_format::
                  rgba8,
              texture_types::format    format          = texture_types::format::rgba,
              texture_types::type      type            = texture_types::type::ubyte,
              bool                     mipmap          = true,
              texture_types::filtering m_filtering_min = texture_types::filtering::linear,
              texture_types::filtering m_filtering_mag = texture_types::filtering::linear,
              void *                   data            = nullptr) noexcept;
    texture_t(texture_t &&rhs) noexcept;
    texture_t(const texture_t &) = delete;
    [[nodiscard]] auto operator  =(texture_t &&rhs) noexcept -> texture_t &;
    [[nodiscard]] auto operator=(const texture_t &) -> texture_t & = delete;
    ~texture_t() noexcept;

    [[nodiscard]] auto id() const noexcept -> std::uint32_t;
    auto               bind(std::int32_t unit = -1) const noexcept -> void;
    static auto        unbind() noexcept -> void;

private:
    std::uint32_t                        m_id{};
    std::uint32_t                        m_width{};
    std::uint32_t                        m_height{};
    texture_types::sized_internal_format m_sized_internal_format = texture_types::sized_internal_format::rgba8;
    texture_types::format                m_format                = texture_types::format::rgba;
    texture_types::type                  m_type                  = texture_types::type::ubyte;
    texture_types::filtering             m_filtering_mag         = texture_types::filtering::linear;
    texture_types::filtering             m_filtering_min         = texture_types::filtering::linear;
    texture_types::sampling              m_wrap_s                = texture_types::sampling::repeat;
    texture_types::sampling              m_wrap_t                = texture_types::sampling::repeat;
    bool                                 m_mipmap                = true;
    // TODO(Marko): remove this attribute
};
} // namespace aech::graphics
