#include "transparent_renderer.hpp"

#include "main.hpp"

aech::graphics::transparent_renderer_t::transparent_renderer_t(render_cache_t *     render_cache,
                                                               camera_t *           camera,
                                                               directional_light_t *dirlight) noexcept
    : m_camera{camera}
    , m_dirlight{dirlight}
    , m_render_cache{render_cache}
{
}

auto aech::graphics::transparent_renderer_t::render_target() const noexcept -> aech::graphics::framebuffer_t *
{
    return m_render_target;
}

auto aech::graphics::transparent_renderer_t::mesh_filter() const noexcept -> aech::graphics::mesh_filter_t
{
    return m_mesh_filter;
}

auto aech::graphics::transparent_renderer_t::update() const noexcept -> void
{
    // setup g-buffer
    m_render_target->bind();
    m_render_cache->set_depth_test(true);
    m_render_cache->set_blend(true);
    m_render_cache->set_blend(blend_func::src_alpha, blend_func::one_minus_src_alpha);
    m_render_cache->set_viewport(0, 0, m_render_target->width(), m_render_target->height());
    m_render_cache->set_cull(true);
    m_render_cache->set_cull_face(cull_face::back);
    m_render_cache->set_depth_func(depth_func::lequal);
    std::array<GLenum, 4> attachments{
        {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3}};
    glDrawBuffers(4, &attachments[0]);

    // sort back to front (roughly)
    std::set<entity_t, decltype(&renderer_t::sort_back_to_front)> entities_sorted{
        &renderer_t::
            sort_back_to_front};
    for (auto entity: entities) {
        if (auto &scene_node = engine.get_component<scene_node_t>(entity); !m_camera->sees(scene_node)) {
            continue; // view frustum culling
        }
        entities_sorted.insert(entity);
    }

    for (auto entity: entities_sorted) {
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
}
