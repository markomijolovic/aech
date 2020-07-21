#include "opaque_shadow_renderer.hpp"
#include "main.hpp"
#include "mesh.hpp"
#include "mesh_filter.hpp"
#include "transform.hpp"
#include "transforms.hpp"

namespace aech::graphics {
opaque_shadow_renderer_t::opaque_shadow_renderer_t(render_cache_t* render_cache, directional_light_t* dirlight)
    : m_dirlight { dirlight }
    , m_render_cache { render_cache }
{
}

framebuffer_t* opaque_shadow_renderer_t::render_target() const
{
    return m_render_target;
}

void opaque_shadow_renderer_t::update()
{
    m_render_target->bind();
    m_render_cache->set_viewport(0, 0, m_render_target->width(), m_render_target->height());
    m_render_cache->clear(clear::color_and_depth_buffer_bit);
    m_render_cache->set_cull(true);
    m_render_cache->set_cull_face(cull_face::back);
    m_render_cache->set_shader(m_material->shader());
    m_material->set_uniforms();

    std::set<entity_t, decltype(&aech::graphics::renderer_t::sort_front_to_back)> entities_sorted {
        &aech::graphics::
            renderer_t::sort_front_to_back
    };
    for (auto entity : entities) {
        entities_sorted.insert(entity);
    }

    for (auto entity : entities_sorted) {
        auto& mesh_filter = engine.get_component<mesh_filter_t>(entity);
        auto& scene_node = engine.get_component<scene_node_t>(entity);
        auto light_view_matrix = math::get_view_matrix(*m_dirlight->transform());

        m_material->shader()->set_uniform("projection", aech::graphics::renderer_t::light_projection);
        m_material->shader()->set_uniform("view", light_view_matrix);
        m_material->shader()->set_uniform("model", scene_node.get_transform());
        mesh_filter.mesh()->draw();
    }
}
} // namespace aech::graphics
