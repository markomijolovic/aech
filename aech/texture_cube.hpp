#pragma once

#include "aech_types.hpp"
#include "texture_types.hpp"

namespace aech::graphics {
// a class that represents a cubemap texture
class texture_cube_t {
public:
    // make unordered_map happy
    texture_cube_t() = default;

    texture_cube_t(std::uint32_t                             width,
                   std::uint32_t                             height,
                   texture_types::sized_internal_format sif    = texture_types::sized_internal_format::rgba8,
                   texture_types::format                f      = texture_types::format::rgba,
                   texture_types::type                  t      = texture_types::type::ubyte,
                   bool                                 mipmap = true,
                   texture_types::filtering             min    = texture_types::filtering::linear,
                   texture_types::filtering             mag    = texture_types::filtering::linear);
    auto operator=(texture_cube_t &&rhs) noexcept -> texture_cube_t &;
    texture_cube_t(texture_cube_t &&rhs) noexcept;
    ~texture_cube_t() noexcept;

    [[nodiscard]] auto id() const -> std::uint32_t;
    auto               generate_face(std::uint32_t index, void *data) const -> void;
    auto               generate_mips() const -> void;

    auto        bind(std::int32_t unit = -1) const -> void;
    static auto unbind() -> void;

private:
    std::uint32_t                             m_id{};
    std::uint32_t                             m_width{};
    std::uint32_t                             m_height{};
    texture_types::sized_internal_format m_sized_internal_format = texture_types::sized_internal_format::rgba8;
    texture_types::format                m_format                = texture_types::format::rgba;
    texture_types::type                  m_type                  = texture_types::type::ubyte;
    texture_types::filtering             m_filtering_min         = texture_types::filtering::linear;
    texture_types::filtering             m_filtering_mag         = texture_types::filtering::linear;
    texture_types::sampling              m_wrap_s                = texture_types::sampling::clamp_to_edge;
    texture_types::sampling              m_wrap_t                = texture_types::sampling::clamp_to_edge;
    texture_types::sampling              m_wrap_r                = texture_types::sampling::clamp_to_edge;
    bool                                 m_mipmap                = true;
};
} // namespace aech::graphics
