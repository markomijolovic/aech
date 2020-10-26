#include "gi_renderer.hpp"

#include "framebuffer_cube.hpp"
#include "framebuffer_library.hpp"
#include "main.hpp"
#include "mat.hpp"
#include "mesh_filter.hpp"
#include "shading_tags.hpp"

#include <array>
#include <iomanip>
#include <iostream>
#include <unordered_set>

aech::graphics::gi_renderer_t::gi_renderer_t(render_cache_t *render_cache, camera_t *camera)
    : m_camera{camera}
    , m_render_cache{render_cache}
{
}

auto aech::graphics::gi_renderer_t::specular_material() const -> aech::graphics::material_t *
{
    return m_ambient_specular_material;
}

auto aech::graphics::gi_renderer_t::diffuse_material() const -> aech::graphics::material_t *
{
    return m_ambient_diffuse_material;
}

auto aech::graphics::gi_renderer_t::render_target() const -> aech::graphics::framebuffer_t *
{
    return m_render_target;
}

auto aech::graphics::gi_renderer_t::add_probe(reflection_probe_t &&probe) -> void
{
    m_reflection_probes.push_back(probe);
}

auto aech::graphics::gi_renderer_t::add_probe(const reflection_probe_t &probe) -> void
{
    m_reflection_probes.push_back(probe);
}

auto aech::graphics::gi_renderer_t::add_probe(light_probe_t &&probe) -> void
{
    m_light_probes.push_back(probe);
}

auto aech::graphics::gi_renderer_t::add_probe(const light_probe_t &probe) -> void
{
    m_light_probes.push_back(probe);
}

auto aech::graphics::gi_renderer_t::bake_probes() -> void
{
    std::clog << "Rendering brdf LUT" << std::endl;
    auto &brdf_fbo = framebuffers["brdf"];

    brdf_fbo.bind();
    m_render_cache->set_viewport(0, 0, brdf_fbo.width(), brdf_fbo.height());
    m_render_cache->set_shader(m_brdf_material->shader());
    m_render_cache->set_depth_test(false);
    m_render_cache->set_blend(false);
    m_render_cache->clear(clear::color_and_depth_buffer_bit);
    m_ndc_quad->draw();
    framebuffer_t::unbind();

    // create preprocessed environment maps for reflection probes
    std::clog << "Baking reflection probes" << std::endl;
    for (std::size_t i{}; i < m_reflection_probes.size(); i++) {
        std::clog << std::setw(6) << std::fixed << std::setprecision(2) << static_cast<float>(i) / m_reflection_probes.size() * 100 << "%" << std::endl;
        const auto pos = m_reflection_probes[i].position();
        create_radiance_cubemap(pos, i);
        create_preprocessed_environment_map(i);
    }

    std::clog << "100.00%" << std::endl;

    // create irradiance maps for light probes
    std::clog << "Baking light probes" << std::endl;
    for (std::size_t i{}; i < m_light_probes.size(); i++) {
        std::clog << std::setw(6) << std::fixed << std::setprecision(2) << static_cast<float>(i) / m_light_probes.size() * 100 << "%" << std::endl;
        const auto pos = m_light_probes[i].position();
        create_radiance_cubemap(pos, i + m_reflection_probes.size());
        create_irradiance_cubemap(i);
    }

    std::clog << "100.00%" << std::endl;
}

auto aech::graphics::gi_renderer_t::create_radiance_cubemap(math::vec3_t position, std::size_t probe_index) -> void
{
    const static auto capture_projection = math::perspective(90, 1, 0.01F, 1000.0F);
    m_render_cache->set_depth_test(true);
    m_render_cache->set_depth_func(depth_func::lequal);

    const std::array capture_views{
        look_at(position, position + math::vec3_t{1, 0, 0}, {0, -1, 0}),
        look_at(position, position + math::vec3_t{-1, 0, 0}, {0, -1, 0}),
        look_at(position, position + math::vec3_t{0, 1, 0}, {0, 0, 1}),
        look_at(position, position + math::vec3_t{0, -1, 0}, {0, 0, -1}),

        look_at(position, position + math::vec3_t{0, 0, 1}, {0, -1, 0}),
        look_at(position, position + math::vec3_t{0, 0, -1}, {0, -1, 0})};

    const auto tex = &resource_manager::texture_cubes["radiance" + std::to_string(probe_index)];
    *tex           = texture_cube_t{
        1024,
        1024,
        texture_types::sized_internal_format::rgb16f,
        texture_types::format::rgb,
        texture_types::type::floating_point};

    const framebuffer_cube_t fbo{tex, 1024, 1024};
    fbo.bind();
    m_render_cache->set_viewport(0, 0, fbo.width(), fbo.height());

    // TODO(Marko): this needs refactoring
    std::unordered_set<entity_t> transparent_entities{};

    for (std::uint32_t i = 0; i < 6; i++) {
        fbo.attach(i);
        render_cache_t::clear(clear::color_and_depth_buffer_bit);

        const auto &view = capture_views[i];
        m_render_cache->set_shader(m_cubemap_capture_material->shader());
        m_cubemap_capture_material->set_uniform("view", view);
        m_cubemap_capture_material->set_uniform("projection", capture_projection);
        for (auto entity: entities) {
            if (engine.has_component<transparent_t>(entity)) {
                transparent_entities.insert(entity);
                continue;
            }

            auto &mesh_filter = engine.get_component<mesh_filter_t>(entity);
            auto &scene_node  = engine.get_component<scene_node_t>(entity);
            for (const auto &texture: mesh_filter.material()->get_textures()) {
                m_cubemap_capture_material->set_texture(texture.first, texture.second.first, texture.second.second);
            }

            m_cubemap_capture_material->shader()->set_uniform("model", scene_node.get_transform());
            m_cubemap_capture_material->set_uniforms();
            mesh_filter.mesh()->draw();
        }

        // render transparent entities
        m_render_cache->set_shader(m_cubemap_capture_transparent_material->shader());
        m_cubemap_capture_transparent_material->set_uniform("view", view);
        m_cubemap_capture_transparent_material->set_uniform("projection", capture_projection);
        m_render_cache->set_blend(true);
        m_render_cache->set_blend(blend_func::src_alpha, blend_func::one_minus_src_alpha);
        for (auto entity: transparent_entities) {
            auto &mesh_filter = engine.get_component<mesh_filter_t>(entity);
            auto &scene_node  = engine.get_component<scene_node_t>(entity);
            for (const auto &texture: mesh_filter.material()->get_textures()) {
                m_cubemap_capture_transparent_material->set_texture(texture.first,
                                                                    texture.second.first,
                                                                    texture.second.second);
            }

            m_cubemap_capture_transparent_material->shader()->set_uniform("model", scene_node.get_transform());
            m_cubemap_capture_transparent_material->set_uniforms();
            mesh_filter.mesh()->draw();
        }

        // render skybox
        m_render_cache->set_cull(false);

        m_render_cache->set_shader(m_cubemap_capture_skybox_material->shader());
        m_cubemap_capture_skybox_material->set_uniform("view", view);
        m_cubemap_capture_skybox_material->set_uniform("projection", capture_projection);
        m_cubemap_capture_skybox_material->set_texture_cube("skybox", m_skybox_mf.material()->get_texture_cube("skybox"), 0);
        m_cubemap_capture_skybox_material->set_uniforms();
        m_skybox_mf.mesh()->draw();
        m_render_cache->set_cull(true);
    }
    fbo.texture()->generate_mips();

    framebuffer_cube_t::unbind();
}

auto aech::graphics::gi_renderer_t::create_preprocessed_environment_map(std::size_t probe_index) -> void
{
    m_render_cache->set_cull(false);
    auto &probe = m_reflection_probes[probe_index];

    const auto prefiltered = &resource_manager::texture_cubes["prefiltered" + std::to_string(probe_index)];
    *prefiltered           = texture_cube_t{
        128,
        128,
        texture_types::sized_internal_format::rgb16f,
        texture_types::format::rgb,
        texture_types::type::floating_point};
    probe.set_prefiltered(prefiltered);

    const framebuffer_cube_t prefilter_fbo{probe.prefiltered(), 128, 128};
    prefilter_fbo.bind();
    m_render_cache->set_shader(m_prefilter_material->shader());
    m_prefilter_material->set_texture_cube("environment_map",
                                           &resource_manager::texture_cubes["radiance" + std::to_string(probe_index)],
                                           0);
    m_prefilter_material->set_uniform("projection", capture_projection);
    const auto levels = static_cast<std::uint32_t>(floor(log2(128)) + 1);

    for (std::uint32_t mip = 0; mip < levels; mip++) {
        const auto width  = static_cast<std::uint32_t>(128 * std::pow(0.5, mip));
        const auto height = width;
        m_render_cache->set_viewport(0, 0, width, height);

        auto roughness = static_cast<float>(mip) / (levels - 1);
        m_prefilter_material->set_uniform("roughness", roughness);
        for (std::uint32_t i = 0; i < 6; i++) {
            m_prefilter_material->set_uniform("view", capture_views[i]);
            m_prefilter_material->set_uniforms();
            prefilter_fbo.attach(i, mip);
            render_cache_t::clear(clear::color_and_depth_buffer_bit);

            m_ndc_cube->draw();
        }
    }

    framebuffer_cube_t::unbind();
    resource_manager::texture_cubes.erase("radiance" + std::to_string(probe_index));
}

auto aech::graphics::gi_renderer_t::create_irradiance_cubemap(std::size_t probe_index) -> void
{
    m_render_cache->set_cull(false);
    auto &probe = m_light_probes[probe_index];

    const auto irradiance = &resource_manager::texture_cubes["irradiance" + std::to_string(probe_index)];
    *irradiance           = texture_cube_t{
        32,
        32,
        texture_types::sized_internal_format::rgb16f,
        texture_types::format::rgb,
        texture_types::type::floating_point};

    probe.set_irradiance(irradiance);
    m_render_cache->set_shader(m_irradiance_capture_material->shader());
    m_irradiance_capture_material->set_texture_cube("environment",
                                                    &resource_manager::texture_cubes["radiance" + std::to_string(probe_index + m_reflection_probes.size())],
                                                    0);

    const framebuffer_cube_t fbo{probe.irradiance(), 32, 32};
    fbo.bind();
    m_render_cache->set_viewport(0, 0, fbo.width(), fbo.height());
    for (std::uint32_t i = 0; i < 6; i++) {
        fbo.attach(i);
        render_cache_t::clear(clear::color_and_depth_buffer_bit);
        m_irradiance_capture_material->shader()->set_uniform("projection", capture_projection);
        m_irradiance_capture_material->shader()->set_uniform("view", capture_views[i]);
        m_irradiance_capture_material->set_uniforms();
        m_ndc_cube->draw();
    }
    fbo.texture()->generate_mips();
    framebuffer_cube_t::unbind();
    resource_manager::texture_cubes.erase("radiance" + std::to_string(probe_index + m_reflection_probes.size()));
}

auto aech::graphics::gi_renderer_t::render_ambient_pass() -> void
{
    m_render_target->bind();
    m_render_cache->set_viewport(0, 0, m_render_target->width(), m_render_target->height());

    m_render_cache->set_cull(true);
    m_render_cache->set_cull_face(cull_face::front);
    render_cache_t::clear(clear::color_and_depth_buffer_bit);
    m_render_cache->set_depth_test(false);
    m_render_cache->set_blend(true);
    m_render_cache->set_blend(blend_func::one, blend_func::one);
    if (m_diffuse_gi) {
        m_render_cache->set_shader(m_ambient_diffuse_material->shader());
        if (renderer.ssao()) {
            renderer.ssao_texture()->bind(5);
            m_ambient_diffuse_material->set_uniform("texture_ssao", 5);
        }
        m_ambient_diffuse_material->set_uniform("dot_product_weight", m_dot_product_weight);
        m_ambient_diffuse_material->shader()->set_uniform("ssao", renderer.ssao());
        m_ambient_diffuse_material->set_uniform("camera_position", m_camera->transform()->position);
        m_ambient_diffuse_material->set_uniform("projection", m_camera->projection());
        m_ambient_diffuse_material->set_uniform("view", math::get_view_matrix(*m_camera->transform()));
        for (auto &probe: m_light_probes) {
            // view frustum culling of probes
            if (!m_camera->sees(*probe.scene_node())) {
                continue;
            }
            m_ambient_diffuse_material->set_uniform("probe_position", probe.position());
            m_ambient_diffuse_material->set_texture_cube("environment_irradiance", probe.irradiance(), 8);
            m_ambient_diffuse_material->set_uniform("model", probe.scene_node()->get_transform());
            m_ambient_diffuse_material->set_uniform("inner_radius", probe.inner_radius());
            m_ambient_diffuse_material->set_uniform("outer_radius", probe.outer_radius());
            m_ambient_diffuse_material->set_uniform("box_min", probe.scene_node()->bounding_box().min_coords);
            m_ambient_diffuse_material->set_uniform("box_max", probe.scene_node()->bounding_box().max_coords);
            m_ambient_diffuse_material->set_uniforms();
            m_ndc_cube->draw();
        }
    }

    if (m_specular_gi) {
        m_render_cache->set_shader(m_ambient_specular_material->shader());
        m_ambient_specular_material->shader()->set_uniform("ssao", renderer.ssao());
        if (renderer.ssao()) {
            renderer.ssao_texture()->bind(5);
            m_ambient_specular_material->set_uniform("texture_ssao", 5);
        }
        m_ambient_specular_material->set_texture("brdf_lut", &framebuffers["brdf"].colour_attachments().front(), 6);
        m_ambient_specular_material->set_uniform("camera_position", m_camera->transform()->position);
        m_ambient_specular_material->set_uniform("projection", m_camera->projection());
        m_ambient_specular_material->set_uniform("view", math::get_view_matrix(*m_camera->transform()));
        for (auto &probe: m_reflection_probes) {
            // view frustum culling of probes
            //if (!m_camera->sees(probe.position(), probe.outer_radius()))
            //{
            //	continue;
            //}
            m_ambient_specular_material->set_uniform("probe_position", probe.position());
            m_ambient_specular_material->set_texture_cube("environment_prefiltered", probe.prefiltered(), 8);
            m_ambient_specular_material->set_uniform("model", probe.scene_node()->get_transform());
            m_ambient_specular_material->set_uniform("box_min", probe.scene_node()->bounding_box().min_coords);
            m_ambient_specular_material->set_uniform("box_max", probe.scene_node()->bounding_box().max_coords);
            m_ambient_specular_material->set_uniforms();
            m_ndc_cube->draw();
        }
    }
}
