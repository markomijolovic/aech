#include "opaque_renderer.hpp"

#include "camera.hpp"
#include "main.hpp"
#include "mesh_filter.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include "scene_node.hpp"
#include "transforms.hpp"

namespace aech::graphics {
auto opaque_renderer_t::render_target() const noexcept -> framebuffer_t *
{
    return m_render_target;
}

auto opaque_renderer_t::draw_skybox() const noexcept -> void
{
    m_render_cache->set_depth_test(true);
    m_render_cache->set_depth_func(depth_func::lequal);
    m_render_cache->set_blend(false);
    m_render_cache->set_shader(&m_skybox_mf.material()->shader());
    m_skybox_mf.material()->shader().set_uniform("view",
                                                 math::get_view_matrix(m_camera->transform()));
    m_skybox_mf.material()->shader().set_uniform("projection", m_camera->projection());
    m_skybox_mf.material()->set_uniforms();
    m_render_cache->set_cull(false);
    m_skybox_mf.mesh()->draw();
    m_render_cache->set_cull(true);
}

opaque_renderer_t::opaque_renderer_t(render_cache_t *render_cache, camera_t *camera) noexcept
    : m_camera{camera}
    , m_render_cache{render_cache}
{
}

auto opaque_renderer_t::update() noexcept -> void
{
    setup_g_buffer();

    // sort front to back (roughly) to take advantage of early z testing
    std::set<entity_t, decltype(&renderer_t::sort_front_to_back)> entities_sorted{
        &renderer_t::sort_front_to_back};
    for (auto entity: entities) {
        if (auto &scene_node = engine.get_component<scene_node_t>(entity); !m_camera->sees(scene_node)) {
            continue; // view frustum culling
        }
        entities_sorted.insert(entity);
    }

    for (auto entity: entities_sorted) {
        draw_entity(entity);
    }
}

auto opaque_renderer_t::setup_g_buffer() const noexcept -> void
{
    m_render_target->bind();
    m_render_cache->set_viewport(0, 0, m_render_target->width(), m_render_target->height());
    render_cache_t::clear(clear::color_and_depth_buffer_bit);
    m_render_cache->set_cull(true);
    m_render_cache->set_depth_test(true);
    m_render_cache->set_blend(false);
    m_render_cache->set_cull_face(cull_face::back);
    m_render_cache->set_depth_func(depth_func::lequal);

    std::array<GLenum, 4> attachments{
        {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3}};
    glDrawBuffers(4, &attachments[0]);
}

auto opaque_renderer_t::draw_entity(entity_t entity) const noexcept -> void
{
    auto  view        = math::get_view_matrix(m_camera->transform());
    auto &scene_node  = engine.get_component<scene_node_t>(entity);
    auto &mesh_filter = engine.get_component<mesh_filter_t>(entity);
    auto  shader      = mesh_filter.material()->shader();
    auto  projection  = m_camera->projection();

    m_render_cache->set_shader(&shader);
    mesh_filter.material()->set_uniforms();
    shader.set_uniform("model", scene_node.get_transform());
    shader.set_uniform("view", view);
    shader.set_uniform("projection", projection);

    mesh_filter.mesh()->draw();
}
} // namespace aech::graphics
