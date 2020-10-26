#include "render_cache.hpp"

aech::graphics::render_cache_t::render_cache_t()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

auto aech::graphics::render_cache_t::set_depth_test(bool enable) -> void
{
    if (m_depth_test != enable) {
        if (m_depth_test = enable) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }
}

auto aech::graphics::render_cache_t::set_depth_func(depth_func func) -> void
{
    if (m_depth_func != func) {
        m_depth_func = func;
        glDepthFunc(static_cast<GLenum>(m_depth_func));
    }
}

auto aech::graphics::render_cache_t::set_blend(bool enable) -> void
{
    if (m_blend != enable) {
        if (m_blend = enable) {
            glEnable(GL_BLEND);
        } else {
            glDisable(GL_BLEND);
        }
    }
}

auto aech::graphics::render_cache_t::set_blend(blend_func source, blend_func dest) -> void
{
    if (m_blend_source != source || m_blend_dest != dest) {
        m_blend_source = source;
        m_blend_dest   = dest;
        glBlendFunc(static_cast<GLenum>(source), static_cast<GLenum>(dest));
    }
}

auto aech::graphics::render_cache_t::set_cull(bool enable) -> void
{
    if (m_cull != enable) {
        if (m_cull = enable) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }
    }
}

auto aech::graphics::render_cache_t::set_cull_face(cull_face face) -> void
{
    if (m_cull_face != face) {
        m_cull_face = face;
        glCullFace(static_cast<GLenum>(m_cull_face));
    }
}

auto aech::graphics::render_cache_t::set_shader(shader_t *shader) -> void
{
    if (m_shader == nullptr || *m_shader != *shader) {
        m_shader = shader;
        m_shader->use();
    }
}

auto aech::graphics::render_cache_t::clear(graphics::clear bit) -> void
{
    glClear(static_cast<GLenum>(bit));
}

auto aech::graphics::render_cache_t::set_viewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) -> void
{
    if (m_viewport_dimensions != std::array{x, y, width, height}) {
        m_viewport_dimensions = {x, y, width, height};
        glViewport(x, y, width, height);
    }
}
