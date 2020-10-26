#include "transparent_shadow_renderer.hpp"

#include "main.hpp"
#include "mesh_filter.hpp"
#include "transforms.hpp"

aech::graphics::transparent_shadow_renderer_t::transparent_shadow_renderer_t(render_cache_t *     render_cache,
                                                                             directional_light_t *directional_light)
    : m_dirlight{directional_light}
    , m_render_cache{render_cache}
{
}

auto aech::graphics::transparent_shadow_renderer_t::update() -> void
{
    auto light_view_matrix = math::get_view_matrix(*m_dirlight->transform());
    m_shadow_map->bind();
    m_render_cache->set_viewport(0, 0, m_shadow_map->width(), m_shadow_map->height());
    m_render_cache->set_cull(true);
    m_render_cache->set_cull_face(cull_face::back);
    m_render_cache->set_shader(m_material->shader());

    std::set<entity_t, decltype(&renderer_t::sort_back_to_front)> entities_sorted{
        &renderer_t::sort_back_to_front};
    for (auto entity: entities) {
        entities_sorted.insert(entity);
    }

    for (auto entity: entities_sorted) {
        auto &mesh_filter = engine.get_component<mesh_filter_t>(entity);
        auto &scene_node  = engine.get_component<scene_node_t>(entity);

        m_material->shader()->set_uniform("projection", renderer_t::light_projection);
        m_material->shader()->set_uniform("view", light_view_matrix);
        m_material->shader()->set_uniform("model", scene_node.get_transform());
        m_material->set_texture("texture_albedo", mesh_filter.material()->get_texture("texture_albedo"), 0);
        m_material->set_uniforms();

        mesh_filter.mesh()->draw();
    }
}
