#pragma once

#include "shader.hpp"

#include <array>

namespace aech::graphics {
enum class cull_face {
    front          = GL_FRONT,
    back           = GL_BACK,
    front_and_back = GL_FRONT_AND_BACK
};

enum class depth_func {
    less   = GL_LESS,
    equal  = GL_EQUAL,
    lequal = GL_LEQUAL,
};

enum class blend_func {
    src_alpha           = GL_SRC_ALPHA,
    one_minus_src_alpha = GL_ONE_MINUS_SRC_ALPHA,
    one                 = GL_ONE
};

enum class clear {
    color_buffer_bit           = GL_COLOR_BUFFER_BIT,
    depth_buffer_bit           = GL_DEPTH_BUFFER_BIT,
    color_and_depth_buffer_bit = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
};

// a class that only changes render state when needed (might be unnecessary)
class render_cache_t {
public:
    render_cache_t() noexcept;

    auto        set_depth_test(bool enable) noexcept -> void;
    auto        set_depth_func(depth_func func) noexcept -> void;
    auto        set_blend(bool enable) noexcept -> void;
    auto        set_blend(blend_func source, blend_func dest) noexcept -> void;
    auto        set_cull(bool enable) noexcept -> void;
    auto        set_cull_face(cull_face face) noexcept -> void;
    auto        set_shader(shader_t *shader) noexcept -> void;
    auto        set_viewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) noexcept -> void;
    static auto clear(clear bit) noexcept -> void;

private:
    bool       m_depth_test{true};
    depth_func m_depth_func{depth_func::less};
    bool       m_blend{true};
    blend_func m_blend_source{blend_func::src_alpha};
    blend_func m_blend_dest{blend_func::one_minus_src_alpha};
    bool       m_cull{true};
    cull_face  m_cull_face{cull_face::back};
    shader_t * m_shader{nullptr};

    std::array<std::uint32_t, 4> m_viewport_dimensions{};
};
} // namespace aech::graphics
